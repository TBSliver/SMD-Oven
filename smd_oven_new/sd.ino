//===========================================================================
//= SD Card Initialisation                                                  =
//===========================================================================

void sdInit() {
  if (!SD.begin(53)) {
    Serial.println("SD Card Initialisation failed!");
    return;
  }
  else
    Serial.println("SD Card Initialised");
    
  noFiles = dirFileCount(root);
}

//===========================================================================
//= File counter for a directory, including actual directories in the file  =
//===========================================================================

uint16_t dirFileCount(const char *path)
{
  uint8_t cnt;
  File dir;
  File f;
  cnt = 0;
  
  dir = SD.open(path);
  for(;;)
  {
    f =  dir.openNextFile();
    if ( !f )
	break;
    cnt++;
    f.close();
    if ( cnt == 0x0ffff )
      break;
  }
  return cnt;
}

//===========================================================================
//= Gets the filename for the current file being looked at.                 =
//= Passes the data to 'fileName' and 'isDir'                               =
//===========================================================================

uint8_t getFileName(const char *path, uint16_t n)
{
  uint16_t c = 0;
  File dir;
  File f;
  
  dir = SD.open(path);
  for(;;)
  {
    f =  dir.openNextFile();
    if ( !f )
      break;
    if ( n == c )
    {
      strcpy(fileName, f.name());
      isDir = f.isDirectory();
      f.close();
      return 1;
    }
    c++;
    f.close();
  }
  return 0;
}

void seekLine(int line) {
  
}

void readStep(int readLine) {
  
}
