void getData (char param1[], char param2[], EthernetClient client)
{
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection: close"));
  client.println();
//  Serial.println("OK");
  switch (param1[0])
  {
    case 't':
      if (param1[1] == '1') 
      {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));             
        client.print(F(","));
        client.print(F("\"Temperature1\":"));
        client.print(F("\""));
        client.print(temp[0]);
        client.print(F("\""));
      }
      else if (param1[1] == '2')
      {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));             
        client.print(F(","));
        client.print(F("\"Temperature2\":"));
        client.print(F("\""));
        client.print(temp[1]);
        client.print(F("\""));
        
      }
      else if (param1[1] == '3')
      {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));             
        client.print(F(","));
        client.print(F("\"Temperature3\":"));
        client.print(F("\""));
        client.print(temp[2]);
        client.print(F("\""));
        
      }
      else if (!param1[1])
      {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));             
        client.print(F(","));
        client.print(F("\"Temperature\":["));
        for (int i  = 0; i < 3; ++i)
        {
          client.print(F("\""));
          client.print(temp[i]);
          client.print(F("\""));
          if (i < 2)
          { 
            client.print(F(","));
          }
        }
        client.print(F("]"));
      }
      else
      {
        failVar(param1, client);
      }
      
 
      break;
      
    case 'm':
      if (param1[1] == 'n') {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));
        client.print(F(",\"Min Temp\":\""));        
        client.print(minTemp);        
        client.print(F("\""));        
      }
      else if (param1[1] == 'x') {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));
        client.print(F(",\"Max Temp\":\""));        
        client.print(maxTemp);        
        client.print(F("\""));        
      }
      else
      {
        failVar(param1, client);
      }
      break;
    case 'f':
      if (param1[1] == 'q') {
        client.print(F("{"));
        client.print(F("\"Status\":\"OK\""));
        client.print(F(",\"Feq\":\""));        
        client.print(period);        
        client.print(F("\""));        
      }
      else
      {
        failVar(param1, client);
      }
      break;
/*    case 's':
      break;
    case 'o':
      break;*/
    default:
      failVar(param1, client);
      break;
  }

//  Serial.println(param1);
//  Serial.println(param2);
  client.println(F("}"));
}
void setData (char param1[], char param2[], EthernetClient client)
{
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Connection: close"));
  client.println();
  switch (param1[0])
  {
    case 'm':
      if (param1[1] == 'n') {
         if (int mn = atoi(param2)) {
          minTemp = mn;
          EEPROM.put(BYTE_MINTEMP, minTemp);
          EEPROM.put(BYTE_MINTEMP + 1, minTemp ^ 0xE5); 
          client.print(F("{"));
          client.print(F("\"Status\":\"SET\""));
          client.print(F(",\"Min Temp\":\""));        
          client.print(minTemp);        
          client.print(F("\""));        
          
        }
      }
      else if (param1[1] == 'x') {
         if (int mx = atoi(param2)) {
          maxTemp = mx;
          EEPROM.put(BYTE_MAXTEMP, maxTemp);
          EEPROM.put(BYTE_MAXTEMP + 1, maxTemp ^ 0xE5); 
          client.print(F("{"));
          client.print(F("\"Status\":\"SET\""));
          client.print(F(",\"Max Temp\":\""));        
          client.print(maxTemp);        
          client.print(F("\""));        
          
        }
      }
      else
      {
        failVar(param1, client);
      }
      break;
    case 'f':
      if (param1[1] == 'q') {
         if (int fq = atoi(param2)) {
          period = fq;
          EEPROM.put(BYTE_PERIOD, period);
          EEPROM.put(BYTE_PERIOD + 1, period ^ 0xE5); 
          client.print(F("{"));
          client.print(F("\"Status\":\"SET\""));
          client.print(F(",\"Feq\":\""));        
          client.print(period);        
          client.print(F("\""));        
          
        }
      }
      else
      {
        failVar(param1, client);
      }
      break;
    case 's':
      break;
    case 'o':
      break;
    default:
      failVar(param1, client);
      break;
  }

//  Serial.println(param1);
//  Serial.println(param2);
  client.println(F("}"));
}
void failVar (char param[], EthernetClient client) 
{
  client.print(F("{"));
  client.print(F("\"Status\":\"FAIL\""));
  client.print(F(",\"Error\":\"Incorrect variable '"));
  client.print(param);
  client.print(F("', available variables is 't1, t2, t3, fq, mx, mn, sw, on'\""));
}

              
              /*client.println(F("HTTP/1.1 200 OK"));
              client.println(F("Content-Type: application/json"));
              client.println(F("Connection: close"));
              client.println();
              client.println(F("{\"Status\":\"OK\"}"));
              if (!HTTP_param[1][0])
              {
                client.println(F("HTTP/1.1 200 OK"));
                client.println(F("Content-Type: application/json"));
                client.println(F("Connection: close"));
                client.println();
                client.print(F("{"));
                client.print(F("\"Status\":\"OK\""));             
                client.print(F(","));
                client.print(F("\"Temperature\":["));
                for (int i  = 0; i < 3; ++i)
                {
                  client.print(F("\""));
                  client.print(temp[i]);
                  client.print(F("\""));
                  if (i < 2)
                  { 
                    client.print(F(","));
                  }
                }
                
  
                client.print(F("]"));
                client.print(F("}"));
              }
            }

          }
          }
*/
