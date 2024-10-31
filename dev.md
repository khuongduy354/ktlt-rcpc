# todo

use string to read socket buffer 

# sample GET messages query 

```js
{
    "@odata.context": "https://graph.microsoft.com/v1.0/$metadata#users('outlook_CF4D926B2DD559E7%40outlook.com')/messages",
    "value": [
        {
            "@odata.etag": "W/\"CQAAABYAAABTSca/7zE/S5ioe2EOI5bwAAaD3nqI\"",
            "bccRecipients": [],
            "body": {
                "content": "<html><head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><style type=\"text/css\" style=\"display:none\">\r\n<!--\r\np\r\n\t{margin-top:0;\r\n\tmargin-bottom:0}\r\n-->\r\n</style></head><body dir=\"ltr\"><div class=\"elementToProof\" style=\"font-family:Aptos,Aptos_EmbeddedFont,Aptos_MSFontService,Calibri,Helvetica,sans-serif; font-size:12pt; color:rgb(0,0,0)\">asdsad</div></body></html>",
                "contentType": "html"
            },
            "bodyPreview": "asdsad",
            "categories": [],
            "ccRecipients": [],
            "changeKey": "CQAAABYAAABTSca/7zE/S5ioe2EOI5bwAAaD3nqI",
            "conversationId": "AQQkADAwATM3ZmYAZS0wZTQxLWMyZjEtMDACLTAwCgAQAOWnEpajOFtKoIUK0voicZ4=",
            "conversationIndex": "AQHbKs+y5acSlqM4W0qghQrS+iJxng==",
            "createdDateTime": "2024-10-30T13:29:07Z",
            "flag": {
                "flagStatus": "notFlagged"
            },
            "from": {
                "emailAddress": {
                    "address": "outlook_CF4D926B2DD559E7@outlook.com",
                    "name": "Duy Huynh"
                }
            },
            "hasAttachments": false,
            "id": "AQMkADAwATM3ZmYAZS0wZTQxLWMyZjEtMDACLTAwCgBGAAADDb71hPR08EuaBlxUX-mb1QcAU0nGv_8xP0uYqHthDiOW8AAAAgEJAAAAU0nGv_8xP0uYqHthDiOW8AAGg9BgvAAAAA==",
            "importance": "normal",
            "inferenceClassification": "focused",
            "internetMessageId": "<CH3PR16MB6404AD4B3FE4D44E68C4959DB9542@CH3PR16MB6404.namprd16.prod.outlook.com>",
            "isDeliveryReceiptRequested": false,
            "isDraft": false,
            "isRead": true,
            "isReadReceiptRequested": false,
            "lastModifiedDateTime": "2024-10-30T13:29:09Z",
            "parentFolderId": "AQMkADAwATM3ZmYAZS0wZTQxLWMyZjEtMDACLTAwCgAuAAADDb71hPR08EuaBlxUX-mb1QEAU0nGv_8xP0uYqHthDiOW8AAAAgEJAAAA",
            "receivedDateTime": "2024-10-30T13:29:08Z",
            "replyTo": [],
            "sender": {
                "emailAddress": {
                    "address": "outlook_CF4D926B2DD559E7@outlook.com",
                    "name": "Duy Huynh"
                }
            },
            "sentDateTime": "2024-10-30T13:29:07Z",
            "subject": "dsa",
            "toRecipients": [
                {
                    "emailAddress": {
                        "address": "outlook_CF4D926B2DD559E7@outlook.com",
                        "name": "Duy Huynh"
                    }
                }
            ],
            "webLink": "https://outlook.live.com/owa/?ItemID=AQMkADAwATM3ZmYAZS0wZTQxLWMyZjEtMDACLTAwCgBGAAADDb71hPR08EuaBlxUX%2Fmb1QcAU0nGv%2B8xP0uYqHthDiOW8AAAAgEJAAAAU0nGv%2B8xP0uYqHthDiOW8AAGg9BgvAAAAA%3D%3D&exvsurl=1&viewmodel=ReadMessageItem"
        }
    ]
}
```

#  

API  
1. Shutdown/Reboot 
Request:
    Subj: Shutdown 
    Content: IP 
Response:  
    Computer is shutting down / Rebooting
 
2. List App / Start/Stop   
Request:  
  Subj: List App 
  Content:   
Response:  
  List of apps  

3. List services / Start/Stop 
Request:  
  Subj: List services 
  Content:  
Response:  
  List of services 

4. Get file / Delete file 
Request: 
  Subj: Get file 
  Content: file path or name  
Response: 
  File raw Content 

5. Screenshot  
6. Start/Stop webcam 
7. 

 
# Design 
## Socket  
Server is always running
Client connect to server through server IP  
after connecting -> server response  

## Mail  
Client and Server must have SDKs connection to same Gmail account  
Server listening to Gmail through a poll-based method  
-> webhooks requires server hosting 









