void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //String temp = "POST /rentacar/pos HTTP/1.1\r\nHost: www.imaginexyz.com:80\r\nConnection: close\r\nAccept: application/json\r\nContent-Type: application/json\r\nContent-Length:19\r\n\r\n{\"Distancia\":20001}";
  //String temp = "{\"Distancia\":20001}";
  //String temp = "{\"title\":\"Hello World!\",\"body\":\"This is my first post!\"}";
  //String temp = "{\"id\":\"1\",\"title\":\"Hello World!\",\"body\":\"This is my first post!\"}";
  String temp = "GET /rentacar/last HTTP/1.1\r\nHost: www.imaginexyz.com:80\r\n\r\n";
  Serial.print(temp);
  Serial.println(temp.length());
  delay(15000);

}
