#include <ESP8266WiFi.h>

#define LED_BUILTIN 2

const char* ssid = "Boba";
const char* password = "123456789";
WiFiServer serverTcp(33245);


void setup() {

  Serial.begin(115200);
  Serial.println("|| Check OK! ||");
  pinMode(LED_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    }
    Serial.print("\n\rWiFi connesso!\n\rIp address:");
    Serial.print(WiFi.localIP());
    serverTcp.begin();
    Serial.print(" Server inizializzato");
    digitalWrite(LED_BUILTIN, HIGH);
}

int contVirgole = 0;
void OrdinamentoIniziali(String arr[])
{
  
    for(int i=0;i<contVirgole-1;i++)
    {
      for(int j=0;j<contVirgole-i-1;j++)
      {
          if(arr[j] > arr[j+1])
          {
                String temp = arr[j]; 
                arr[j] = arr[j+1]; 
                arr[j+1] = temp; 
          }
      }
    }


}

void OrdinamentoLunghezza(String arr[])
{
    for(int i=0;i<contVirgole-1;i++)
    {
      for(int j=0;j<contVirgole-i-1;j++)
      {
          if(arr[j].length() > arr[j+1].length())
          {
                String temp = arr[j]; 
                arr[j] = arr[j+1]; 
                arr[j+1] = temp; 
          }
      }
    }
}





void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  WiFiClient clientTcp = serverTcp.available();
  if (!clientTcp) return;
  Serial.println(" --> Client connesso <-- ");
  digitalWrite(LED_BUILTIN, LOW);
  while (true) //fino a che il client non si disconnette
  {
    while (!clientTcp.available()) { delay(1); }
    String message = "";
    while (message.indexOf("\r")==-1)
    {
      String subMessage = clientTcp.readString();
      message = message + subMessage;
    }
    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    message.replace("\n","");
    message.replace("\r","");
    Serial.println("Messaggio ricevuto: ");
    Serial.println(message);
    String answer = "";
    if (message.indexOf("CLOSE")>-1) {
      Serial.println("Client request disconnection");
      clientTcp.stop();
      return;
    }
    else if(message.indexOf("GET") >-1)
    {

      int start = message.indexOf(".");
      for(int i=start+1;i<message.length();i++)
      {
          char lett = message[i];
        if(lett == ',' || lett == ']')
        {
          contVirgole+=1;
        }
      }
      
      String parole[contVirgole];
      String parola = "";
      int index = 0;
      for(int i=start+1;i<message.length();i++)
      {
        char lett = message[i];
        if(lett != ',' && lett != ']')
        {
          parola+= lett;
        }
        else
        {
          parole[index] = parola;
          index+=1;
          parola = "";
        }
      }

String modOrd = "";

      for(int i=message.indexOf("[")+1;i<message.indexOf(".");i++)
      {
        modOrd += message[i];
      }

      if(modOrd=="iniziali")
      {
        OrdinamentoIniziali(parole);
      }
      else if(modOrd=="lunghezza")
      {
        OrdinamentoLunghezza(parole);
      }
      String risposta = "";
      for(int i=0;i<contVirgole;i++)
      {
        clientTcp.println(parole[i] );
      }
      contVirgole = 0;
    }
    
    else if (message.length() > 0)
    {
      answer = "ECHO" + message;
    }
    clientTcp.println(answer);
    delay(1);
  }
}
