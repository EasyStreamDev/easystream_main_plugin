#include "Transcriptor.hpp"
#include "../../PluginManager.hpp"

es::transcript::Transcriptor::Transcriptor() : _isRunning(false), _tcpCli(_isRunning)
{
}

es::transcript::Transcriptor::~Transcriptor()
{
}

void es::transcript::Transcriptor::run(void *pm)
{
    _isRunning = true;

    while (!_tcpCli.connectToServer())
    {
        std::cerr << "[EASYSTREAM TRANSCRIPTOR]: could not connect to echostra! trying again in 1s" << std::endl;
        // return;
        _tcpCli.connectToServer();
    }

    _tcpCli.setPushFunc(
        [this](const json &req)
        {
            {
                std::unique_lock lock(_mtx);
                _tasks.push(req);
            }
            _cVar.notify_all();
        });

    _threads.emplace_back(std::thread(&AsioClient::readMessage, std::ref(_tcpCli)));

    _pluginManager = static_cast<PluginManager *>(pm);

    while (_isRunning)
    {
        {
            std::unique_lock lock(_mtx);
            _cVar.wait(lock, [this]()
                       { return !_isRunning || (_tasks.size() > 0); });
        }
        executeRequest();
    }
}

void es::transcript::Transcriptor::stop()
{
    _isRunning = false;
    _cVar.notify_all();
}

void es::transcript::Transcriptor::enableSubtitlesOnMic(const char *micName, const std::string &language)
{
    obs_source_t *source = obs_get_source_by_name(micName);

    auto it = _recorders.emplace(micName, new micsInfo());

    if (it.second)
    {
        _recorders[micName]->_recorder = new obs::SourceRecorder();
        _recorders[micName]->_pusher = new DataStream();
        _recorders[micName]->_recorder->init(source, micName);
    }

    json message{
        {"command", "createSTTStream"},
        {"params", {{"bit_depth", 16}, {"sample_rate", 48000}, {"stereo", true}, {"mic_id", it.first->first}, {"language", language}}}};
    _tcpCli.writeMessage(message.dump().c_str());
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
        {"data", {}}};

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
    while (_tasks.size())
    {
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
    // _recorders[nameMic]->_pusher->getPort();
    _recorders[nameMic]->_pusher->setPort(i);
    _recorders[nameMic]->_recorder->setPushFunc(
        [this](const char *name, std::string audio)
        {
            es::transcript::micsInfo *tmp = nullptr;
            {
                std::unique_lock lock(_mtxPush);
                tmp = _recorders[name];
            }
            tmp->_pusher->sendMessage(std::move(audio));
        });
    _recorders[nameMic]->_pusher->connectToTranscription();
    _recorders[nameMic]->_recorder->setActive(true);
}

void es::transcript::Transcriptor::pushAudio(const char *name, const std::string &audio)
{
    _recorders[name]->_pusher->sendMessage(audio);
}

void es::transcript::Transcriptor::publishTranscript(json req)
{
    // const char *micId = req["mic_id"].get<std::string>().c_str();
    // std::string _transcription = req["transcript"];
    _pluginManager->GetSubtitlesManager()->pushSubtitles(req["mic_id"].get<std::string>(), req["transcript"]);
    _pluginManager->GetAreaMain()->AddWords(req["transcript"]);
}

void es::transcript::Transcriptor::disableSubtitlesOnMic(std::string micName)
{
    {
        std::unique_lock lock(_mtxPush);
        _recorders[micName]->_nbTextFieldLinked -= 1;


        if (_recorders[micName]->_nbTextFieldLinked <= 0 && !_recorders[micName]->_isEnableWordDetection) {
            _recorders[micName]->_recorder->setActive(false);
            _recorders[micName]->_pusher->disconnectSocket();
        }
    }
}