#include "Transcriptor.hpp"
#include "../../PluginManager.hpp"

es::transcript::Transcriptor::Transcriptor(): _isRunning(false), _tcpCli(_isRunning)
{

}

es::transcript::Transcriptor::~Transcriptor()
{

}

void es::transcript::Transcriptor::run(void *pm)
{
    _isRunning = true;
    

    if (!_tcpCli.connectToServer()) {
        std::cerr << "[EASYSTREAM TRANSCRIPTOR]: could not connect to echostra" << std::endl;
        return;
    }

    _tcpCli.setPushFunc([this](const json &req) {
        {
            std::unique_lock lock(_mtx);
            _tasks.push(req);
        }
        _cVar.notify_all();
    });

    _threads.emplace_back(std::thread(&AsioClient::readMessage, std::ref(_tcpCli)));

    _pluginManager = static_cast<PluginManager *>(pm);

    while(_isRunning) {
        std::cerr << "Running here" << std::endl;
        {
            std::unique_lock lock(_mtx);
            _cVar.wait(lock, [this]() {
                return !_isRunning || (_tasks.size() > 0);
            });
        }
        executeRequest();
    }
}

void es::transcript::Transcriptor::stop()
{
    _isRunning = false;
    _cVar.notify_all();
}

void es::transcript::Transcriptor::enableSubtitlesOnMic(const char *micName)
{
    obs_source_t *source = obs_get_source_by_name(micName);

    auto it = _recorders.emplace(micName, new micsInfo());

    // std::cerr << "Transcriptor, succesfully created transcription for mic " << micName << std::endl;
    if (it.second) {
        _recorders[micName]->_recorder = new obs::SourceRecorder();
        _recorders[micName]->_pusher = new DataStream();
        _recorders[micName]->_recorder->init(source, micName);
    }

    json message{
        {"command", "createSTTStream"},
        {"params", {
            {"bit_depth", 16},
            {"sample_rate", 48000},
            {"stereo", true},
            {"mic_id", it.first->first}
        }}
    };
    _tcpCli.writeMessage(message.dump().c_str());
    std::cerr << "Transcriptor, succesfully created transcription for mic " << micName << std::endl;
    // TcpClient send message for new transcription
}

// void es::transcript::Transcriptor::startNewRecorderByUuid(const char *uuid)
// {
//     // obs_source_t *source = obs_get_source_by_uuid(uuid);

//     // auto it = _recorders.emplace(obs_source_get_name(source), source);
//     // _threads.emplace_back(std::thread(&obs::SourceRecorder::run, std::ref(it.first->second)));
//     // send message to python server to start recording
// }

// void es::transcript::Transcriptor::startNewRecorderByName(const char *name)
// {
//     // auto it = _recorders.emplace(name, obs_get_source_by_name(name));
//     // _threads.emplace_back(std::thread(&obs::SourceRecorder::run, std::ref(it.first->second)));
// }

json es::transcript::Transcriptor::getAllActiveMics()
{
    json result = {
        {"data", {}}
    };

    // for (const auto &it: _recorders) {
    //     if (it.second.isActive())
    //         result["data"] += json{
    //             {"micName", it.first}
    //         };
    // }
    return result;
}

void es::transcript::Transcriptor::executeRequest()
{
    while (_tasks.size()) {
        json req = _tasks.front();
        if (req["type"] == "createSTTStream")
            enableMics(req);
        else if (req["type"] == "transcript")
            publishTranscript(req);
        _tasks.pop();
    }
}

void es::transcript::Transcriptor::enableMics(json req)
{
    std::string nameMic = req["mic_id"].get<std::string>();

    int i = req.at("port");
    _recorders[nameMic]->_pusher->getPort();
    _recorders[nameMic]->_pusher->setPort(i);
    _recorders[nameMic]->_recorder->setPushFunc([this] (const char *name, const std::string &audio) { 
        es::transcript::micsInfo *tmp = nullptr;
        {
            std::unique_lock lock(_mtxPush);
            tmp = _recorders[name];
        }
        tmp->_pusher->sendMessage(audio.c_str());
    });
    _recorders[nameMic]->_pusher->connectToTranscription();
    _recorders[nameMic]->_recorder->setActive(true);
}

void es::transcript::Transcriptor::pushAudio(const char *name, const std::string &audio)
{
    _recorders[name]->_pusher->sendMessage(audio.c_str());
}

void es::transcript::Transcriptor::publishTranscript(const json &req)
{
    // const char *micId = req["mic_id"].get<std::string>().c_str();
    // std::string _transcription = req["transcript"];
    _pluginManager->GetSubtitlesManager()->pushSubtitles(req["mic_id"].get<std::string>().c_str(), req["transcript"]);
}

void es::transcript::Transcriptor::disableSubtitlesOnMic(const char *micName)
{
    _recorders[micName]->_nbTextFieldLinked -= 1;

    if (_recorders[micName]->_nbTextFieldLinked == 0 && _recorders[micName]->_isEnableWordDetection)
        _recorders[micName]->_recorder->setActive(false);
}