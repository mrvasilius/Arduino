int parseReq (char *c)
{
  int err;
  switch (stepReq)
  {
    case 0:
      if (c != ' ')
      {
        if (req_index < REQ_METHOD_BUF_SZ)
        {
          HTTP_req_method[req_index] = c;
          req_index++;
          err = 0;          
        }
      }
      else
      {
        req_index = 0;
        if (StrContains(HTTP_req_method, "GET"))
        {
          stepReq = 1;
          err = 0;
        }
        else
        {
          stepReq = 2;
          err = 1;
        }
      }
      break;
    case 1:
      if (c != '/')
      {
        if (c == ' ')
        {
          stepReq = 2;
          err = 0;
          break;
        }
        if (req_index < REQ_BUF_SZ - 1)
        {
          HTTP_param[param][req_index] = c;
//          Serial.println(param);
//          Serial.println(req_index);
//          Serial.println(HTTP_param[param]);
          req_index++;
          err = 0;
        }
        else
        {
          err = 2;
        }

      }
      else
      {
        param++;
        req_index = 0;
        err = 0;
      }
      break;
    default:
      break;
  }
  if (param >= 3)
  {
    stepReq = 2;
    err = 0;
  }
  return err;
}
void StrClear(char *str, char length)
{
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}
bool StrContains(char *str, char *sfind)
{
  char found = 0;
  char index = 0;
  char len;
  len = strlen(str);
  if (strlen(sfind) > len) {
    return false;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return true;
      }
    }
    else {
      found = 0;
    }
    index++;
  }
  return false;
}
