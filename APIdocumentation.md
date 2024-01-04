# Server endpoint documentation

## **Get requests**

<br>

### **Getting all microphones data**

- **Description**  
  Le client demande la liste de tous les micros ainsi que des données disponibles à leurs sujet.

- **Request**

```json
{
  "command": "/microphones/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "mics": [
            {
                "isActive": "boolean",
                "level": "integer",
                "micKind": "string",
                "micName": "string",
                "unversionedmicKind": "string",
                "uuid": "string",
            },
            ...
        ]
    }
}
```

<br>

---

### **Getting all scenes basic data**

- **Description**  
  Le client demande la liste de toutes les scenes ainsi que des données disponibles à leurs sujet.

- **Request**

```json
{
  "command": "/scenes/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "scenes": [
            {
                "name": "string",
                "uuid": "string",
            },
            ...
        ]
    }
}
```

<br>

---

### **Getting all text fields basic data**

- **Description**  
  Le client demande la liste de tous les champs texte ainsi que certaines données à leurs sujet.  
  Les données renvoyées à leur sujet sont les suivantes:

  - Le nom du champ texte (`name`) - Le nom de la scène à laquelle le champ texte appartient (`parent_scene`) - L'identifiant unique du champ texte (`uuid`)

- **Request**

```json
{
  "command": "/text-fields/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "text_fields": [
            {
                "name": "string",
                "parent_scene": "string",
                "uuid": "string",
            },
            ...
        ]
    }
}
```

<br>

---

### **Getting all display sources basic data**

- **Description**  
  Le client demande la liste de toutes les sources affichant un flux d'image ainsi que certaines données à leurs sujet.  
  Les données renvoyées à leur sujet sont les suivantes:

  - Le nom de la source d'affichage de flux vidéo (`name`)
  - Le nom de la scène à laquelle l'élément (`parent_scene`)
  - L'identifiant unique de l'élément (`uuid`)

- **Request**

```json
{
  "command": "/display-sources/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "display_sources": [
            {
                "name": "string",
                "parent_scene": "string",
                "uuid": "string",
            },
            ...
        ]
    }
}
```

<br>

---

### **Getting all links between mics and display sources**

- **Description**  
  Le client demande la liste de tous les liens entre microphones et sources visuelles.  
  Les données renvoyées à leur sujet sont les suivantes:

  - L'identifiant unique (UUID) du microphone concerné (`mic_id`)
  - La liste contenant les identifiants uniques des sources visuelles reliées au microphone correspondant au champ `mic_id` (`display_sources_ids`)

- **Request**

```json
{
  "command": "/mtdsis/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "length": "integer",
        "links": [
            {
                "mic_id": "uuid",
                "display_sources_ids": ["uuid", ...],
            },
            ...
        ]
    }
}
```

<br>

---

### **Getting action / reaction couples**

- **Description**  
  Récupération des données de toutes les couples actions / réaction enregistrés au niveau du serveur (plugin).

- **Request**

```json
{
  "command": "/areas/get"
}
```

- **Response**

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
                        ... // Depends on action type
                    }
                },
                "reaction": {
                    "name": "string",
                    "reactionId": "integer",
                    "type": "reaction_type",
                    "params": {
                        ... // Depends on reaction type
                    }
                }
            },
            ... // Next element
        ]
    }
}
```

---

### **Getting subtitles settings**

- **Description**  
  Récupération des paramètres de sous-titrage enregistrés au niveau du serveur (plugin).

- **Request**

```json
{
  "command": "/subtitles/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
      "length": "integer",
      "text_fields": [
        {
          "uuid": "string",
          "name": "string",
          "linked_mics": ["MicA", "MicB"]
        },
        ... // Next element
      ]
    }
}
```

---

### **Getting Easystream settings**

- **Description**  
  Récupération des paramètres effectifs du plugin.

- **Request**

```json
{
  "command": "/profile/get"
}
```

- **Response**

```json
{
    "statusCode": "integer",
    "message": "string",
    "data": {
        "easystream": {
            "areas": {
                "length": "integer",
                "actReacts": [
                    {
                        "actReactId": "integer",
                        "isActive": "boolean",
                        "action": {
                            "actionId": "integer",
                            "type": "action_type",
                            "params": {
                                ... // Depends on action type
                            }
                        },
                        "reaction": {
                            "name": "string",
                            "reactionId": "integer",
                            "type": "reaction_type",
                            "params": {
                                ... // Depends on reaction type
                            }
                        }
                    },
                    ... // Next element
                ]
            },
            "subtitles": {
                "length": "integer",
                "text_fields": [
                    {
                        "uuid": "string",
                        "name": "string",
                    },
                    ... // Next element
                ]
            },
            "compressors": {
                "length": "integer",
                "mics": [
                    {
                        "micName": "string",
                        "level": "integer",
                        "isActive": "boolean",
                    },
                    ... // Next element
                ]
            }
        },
        "obs": {
            // Coming soon
        }
    }
}
```

<br>

## **Set requests**

---

### **Setting compressor strength**

- **Description**  
  Mise à jour de la force d'un compresseur (lié à une entrée audio spécifique).

- **Request**

```json
{
    "command": "/microphones/auto-leveler/set",
    "params": {
        "micName": "string",
        "level": "integer",
        "isActive": "boolean",
    }
},
```

- **Response**

```json
{
  "statusCode": "integer",
  "message": "string"
}
```

<br>

---

### **Enabling / Disabling subtitles**

- **Description**  
  Activer / Désactiver les sous-titres transcrit de l'entrée d'un microphone spécifique.

- **Request**

```json
{
  "command": "/subtitles/set",
  "params": {
    "uuid": "uuid", // Correspondant au textfield d'affichage.
    "linked_mics": ["MicA", "MicB"], // Nom des microphones à partir desquels créer les sous-titres.
    "language": "string" (optional), // IETF language tag, translates transciption to the specified language.
  }
}
```

- **Response**

```json
{
  "statusCode": "integer",
  "message": "string"
}
```

<br>

---

### **Link a microphone to several display sources**

- **Description**  
   Créer un lien [MTDSIS](#mtdsis) entre un microphone et une liste de sources visuelles.

- **Request**

```json
{
  "command": "/mtdsis/create",
  "params": {
    "mic_id": "uuid",
    "display_sources_ids": ["uuid", ...],
  }
}
```

- **Response**

```json
{
  "statusCode": "integer",
  "message": "string"
}
```

<br>

---

### **Setting an action / reaction couple**

- **Description**  
  Mise en place d'un couple action/reaction au niveau du plugin.  
  Lorsque l'action est détectée, la réaction est déclenchée automatiquement.

- **Request**

```json
{
    "command": "/areas/create",
    "params": {
        "action": {
            "type": "action_type",
            "params": {
                ... // Depends on action type
            }
        },
        "reaction": {
            "name": "string",
            "type": "reaction_type",
            "params": {
                ... // Depends on reaction type
            }
        }
    }
}
```

- **Response**

```json
{
  "statusCode": "integer",
  "message": "string",
  "data": {
    "actionId": "integer",
    "reactionId": "integer",
    "actReactId": "integer"
  }
}
```

<br>

## **Remove and Update requests**

### **Remove link between a microphone and several display sources**

- **Description**  
   Suppression d'un lien [MTDSIS](#mtdsis) entre un microphone et une liste de sources visuelles.

- **Request**

```json
{
  "command": "/mtdsis/remove",
  "params": {
    "mic_id": "uuid"
  }
}
```

### **Remove an action / reaction couple**

- **Description**  
  Suppression d'un couple action/reaction au niveau du plugin.

- **Request**

```json
{
  "command": "/areas/remove",
  "params": {
    "actReactId": "integer"
  }
}
```

- **Response**

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

- **Description**  
  Mise à jour d'une action côté plugin.  
  L'action peut changer de type et / ou de paramètres.  
  Une fois mise à jour, l'action reste reliée à sa/ses réaction(s).  
  Si la requête est invalide, l'action reste inchangée.

- **Request**

```json
{
    "command": "/areas/action-update",
    "params": {
        "actionId": "integer",
        "type": "action_type",
        "params": {
            ... // Depends on action type
        }
    }
}
```

- **Response**

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

- **Description**  
  Mise à jour d'une réaction côté plugin.  
  La réaction peut changer de type et / ou de paramètres.  
  Une fois mise à jour, la réaction reste reliée à son/ses action(s).  
  Si la requête est invalide, l'action reste inchangée.

- **Request**

```json
{
    "command": "/areas/reaction-update",
    "params": {
        "name": "string",
        "reactionId": "integer",
        "type": "reaction_type",
        "params": {
            ... // Depends on reaction type
        }
    }
}
```

- **Response**

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

## _Subscribe to broadcast_

- **Request**

```json
{
  "command": "/broadcast/subscribe",
  "params": {
    "enable": "bool"
  }
}
```

- **Response**

```json
{
  "statusCode": "integer",
  "message": "string"
}
```

## _Remote changes_

**Areas Update**

```json
{
    "statusCode": 201,
    "message": "BROADCAST",
    "data": {
        "type": "areasChanged",
        "length": "integer",
        "actReacts": [
            {
                "actReactId": "integer",
                "isActive": "boolean",
                "action": {
                    "actionId": "integer",
                    "type": "action_type",
                    "params": {
                        ... // Depends on action type
                    }
                },
                "reaction": {
                    "name": "string",
                    "reactionId": "integer",
                    "type": "reaction_type",
                    "params": {
                        ... // Depends on reaction type
                    }
                }
            },
            ... // Next element
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
        "type": "compressorSettingsChanged",
        "length": "integer",
        "mics": [
            {
                "micName": "string",
                "level": "integer",
                "isActive": "boolean",
            },
            ... // Next element
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
    "type": "subtitlesSettingsChanged",
    "enable": "boolean",
    "language": "string" // IETF language tag
  }
}
```

## _OBS: Scene updates_

**Scene Created**

```json
{
  "statusCode": 201,
  "message": "BROADCAST",
  "data": {
    "type": "sceneCreated",
    "name": "string",
    "uuid": "string"
  }
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
  }
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
  }
}
```

## _OBS: Audio source updates_

**Audio Source Created**

```json
{
  "statusCode": 201,
  "message": "BROADCAST",
  "data": {
    "type": "audioSourceCreated",
    "name": "string",
    "uuid": "string",
    "kind": "string",
    "unversioned_kind": "string"
  }
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
    "kind": "string",
    "unversioned_kind": "string"
  }
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
  }
}
```

<br>
<br>

# **Actions / Reactions**

## _Actions_

**Word detection**

```json
{
  "type": "WORD_DETECT",
  "params": {
    "words": ["word1", "word2", "..."]
  }
}
```

**Application launch**

```json
{
  "type": "APP_LAUNCH",
  "params": {
    "app_name": "app_identifier"
  }
}
```

**Key pressed**

```json
{
  "type": "KEY_PRESSED",
  "params": {
    "key": "key_identifier"
  }
}
```

---

## _Reactions_

**Scene switch**

```json
{
  "type": "SCENE_SWITCH",
  "params": {
    "name": "scene_name", // Not necessary for SET or UPDATE requests
    "uuid": "scene_unique_identifier"
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
  "type": ["START_REC", "STOP_REC"], // One of ...
  "params": {
    "delay": "int" // In seconds
  }
}
```

**Start/Stop streaming**

```json
{
  "type": ["START_STREAM", "STOP_STREAM"], // One of ...
  "params": {
    "delay": "int" // In seconds
  }
}
```

<br>
<br>

---

# Lexique

### MTDSIS

Acronyme de "Mic To Display Sources Intelligent Switch".<br>
Méchanisme de changement d'affichage des sources visuelles en fonction du microphone le plus actif.
