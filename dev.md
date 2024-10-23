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









