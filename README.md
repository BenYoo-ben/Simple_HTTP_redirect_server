#### Simple HTTP redirect server  
  
  [Ko]  
  TCP 연결에 반응하여 HTTP 302 Redirect와 비슷한 기능을 하는 서버 프로그램입니다.  
  HTTP 302 Redirection 의 경우 cross-Domain Security 문제가 발생할 수 있습니다.  
  이 서버의 경우 redirect할 URL로 libcurl을 통해 직접 web page를 요청하여   
  그 결과를 클라이언트에게 되돌려주는 기능을 하여 이런 문제점을 보완했습니다.   
    
  [Eng]  
  This server program responds to TCP connections to perform URL redirection like an action of HTTP response code 302.  
  In case of HTTP 302 redirection, security problems can be faced if you try to access different domains.  
  This server program makes request to URL to be redirected using libcurl then passes result to clients.  
  In this way cross-domain security problems can be avoided.  


  #### Screenshots  
  ![n1](https://user-images.githubusercontent.com/57353430/138787154-b3a6377e-8a5e-4c24-95df-7d7806a1e6f0.PNG)
  ##### Redirected to requested URL  
![n2](https://user-images.githubusercontent.com/57353430/138787191-d7ec851f-2492-43fa-814a-8e320650e522.PNG)
