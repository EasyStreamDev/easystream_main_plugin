# Server endpoint documentation


## **Get requests**
<br>

### **Getting all microphones data**

* **Description**  
Le client demande la liste de tous les micros ainsi que des données disponibles à leurs sujet.  

* **Request**  
```json
{
    "command": "getAllMics",
}
```

* **Response**  
```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "mics": [
            {
                "micName": "string",
                "level": "integer",
                "isActive": "boolean",
            },
            ...
        ]
    }
}
```

<br>

---
### **Getting action / reaction couples**

* **Description**  
Récupération des données de toutes les couples actions / réaction enregistrés au niveau du serveur (plugin).  

* **Request**
```json
{
    "command": "getActReactCouples",
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "actReacts": [
            {
                "actReactId": "integer",
                "isActive": "boolean",
                "action": {
                    "actionId": "integer",
                    "type": "action_type",
                    "params": {
                        "..." // Depends on action type
                    }
                },
                "reaction": {
                    "name": "string",
                    "reactionId": "integer",
                    "type": "reaction_type",
                    "params": {
                        "..." // Depends on reaction type
                    }
                }
            },
            "..." // Next element
        ]
    }
}
```


<br>

## **Set requests**

---
### **Setting compressor strength**
* **Description**  
Mise à jour de la force d'un compresseur (lié à une entrée audio spécifique).  

* **Request**  
```json
{
    "command": "setCompressorLevel",
    "params": {
        "micName": "string",
        "level": "integer",
        "isActive": "boolean",
    }
},
```

* **Response**  
```json
{
    "statusCode": "integer",
    "message": "string",
}
```

<br>

---
### **Enabling / Disabling subtitles**
* **Description**  
Activer / Désactiver les sous-titres transcrit de l'entrée d'un microphone spécifique.  
Le paramètre `language` n'est pris en compte que si la transciption est activée.  

* **Request**
```json
{
    "command": "setSubtitles",
    "params": {
        "enable": "boolean",
        "language": "string", // IETF language tag
    }
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string",
}
```

<br>

---
### **Setting an action / reaction couple**  

* **Description**  
Mise en place d'un couple action/reaction au niveau du plugin.  
Lorsque l'action est détectée, la réaction est déclenchée automatiquement.  

* **Request**
```json
{
    "command": "setActionReaction",
    "params": {
        "action": {
            "type": "action_type",
            "params": {
                "..."
            }
        },
        "reaction": {
            "name": "string",
            "type": "reaction_type",
            "params": {
                "..."
            }
        }
    }
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "actionId": "integer",
        "reactionId": "integer",
        "actReactId": "integer",
    }
}
```
 
<br>

## **Remove and Update requests**

### **Remove an action / reaction couple**  

* **Description**  
Suppression d'un couple action/reaction au niveau du plugin.  

* **Request**
```json
{
    "command": "removeActReact",
    "params": {
        "actReactId": "integer",
    }
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "actReactId": "integer"
    }
}
```

<br>

---
### **Updating an action**  

* **Description**  
Mise à jour d'une action côté plugin.  
L'action peut changer de type et / ou de paramètres.  
Une fois mise à jour, l'action reste reliée à sa/ses réaction(s).  
Si la requête est invalide, l'action reste inchangée.

* **Request**
```json
{
    "command": "updateAction",
    "params": {
        "actionId": "integer",
        "type": "action_type",
        "params": {
            "..."
        }
    }
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "actionId": "integer"
    }
}
```

<br>

---
### **Update reaction**  

* **Description**  
Mise à jour d'une réaction côté plugin.  
La réaction peut changer de type et / ou de paramètres.  
Une fois mise à jour, la réaction reste reliée à son/ses action(s).  
Si la requête est invalide, l'action reste inchangée.

* **Request**
```json
{
    "command": "updateReaction",
    "params": {
        "name": "string",
        "reactionId": "integer",
        "type": "reaction_type",
        "params": {
            "..."
        }
    }
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "reactionId": "integer"
    }
}
```

<br>
<br>

# **Broadcast**

## *Subscribe to broadcast*

* **Request**
```json
{
    "command": "subscribeBroadcast",
    "params": {
        "enable": "bool"
    }
}
```

* **Response**
```json
{
    "statusCode": "integer",
    "message": "string"
}
```

## *Remote changes*

**Areas Update**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "length": "integer",
        "actReacts": [
            {
                "actReactId": "integer",
                "isActive": "boolean",
                "action": {
                    "actionId": "integer",
                    "type": "action_type",
                    "params": {
                        "..." // Depends on action type
                    }
                },
                "reaction": {
                    "name": "string",
                    "reactionId": "integer",
                    "type": "reaction_type",
                    "params": {
                        "..." // Depends on reaction type
                    }
                }
            },
            "..." // Next element
        ]
    },
}
```

**Compressors Update**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "length": "integer",
        "mics": [
            {
                "micName": "string",
                "level": "integer",
                "isActive": "boolean",
            },
            "..." // Next element
        ]
    },
}
```

**Subtitles Update**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "enable": "boolean",
        "language": "string", // IETF language tag
    },
}
```


## *OBS: Scene updates*

**Scene Created**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "sceneCreated",
        "name": "string",
        "uuid": "string"
    },
}
```

**Scene Removed**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "sceneRemoved",
        "name": "string",
        "uuid": "string"
    },
}
```

**Scene Name Changed**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "sceneNameChanged",
        "name": "string",
        "oldName": "string",
        "uuid": "string"
    },
}
```

## *OBS: Audio source updates*

**Audio Source Created**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "audioSourceCreated",
        "name": "string",
        "uuid": "string",
        "type": "string"
    },
}
```

**Audio Source Removed**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "audioSourceRemoved",
        "name": "string",
        "uuid": "string",
        "type": "string"
    },
}
```

**Audio Source Name Changed**
```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "audioSourceNameChanged",
        "name": "string",
        "oldName": "string",
        "uuid": "string"
    },
}
```

<br>
<br>

# **Actions / Reactions**

## *Actions*

**Word detection**
```json
{
    "type": "WORD_DETECT",
    "params": {
        "words": ["word1", "word2", "..."]
    },
}
```

**Application launch**
```json
{
    "type": "APP_LAUNCH",
    "params": {
        "app_name": "app_identifier"
    },
}
```

**Key pressed**
```json
{
    "type": "KEY_PRESSED",
    "params": {
        "key": "key_identifier"
    },
}
```

---
## *Reactions*

**Scene switch**
```json
{
    "type": "SCENE_SWITCH",
    "params": {
        "scene": "scene_identifier"
    }
}
```

**Toggle audio compressor**
```json
{
    "type": "TOGGLE_AUDIO_COMPRESSOR",
    "params": {
        "audio-source": "audio_source_identifier",
        "toggle": "bool"
    }
}
```

**Start/Stop recording**
```json
{
    "type": "START_REC" | "STOP_REC",
    "params": {
        "delay": "int" // In seconds
    }
}
```

**Start/Stop streaming**
```json
{
    "type": "START_STREAM" | "STOP_STREAM",
    "params": {
        "delay": "int" // In seconds
    }
}
```
