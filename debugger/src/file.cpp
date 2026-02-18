#include "file.h" 

size_t BootCount(const char *idxDir, const char *idxFile) {
    String idxPath = String(idxDir) + idxFile;

    if (!SD.exists(idxPath)) {
        Serial.println("Error: index file does not exist");
        return (size_t)-1;
    }

    File f = SD.open(idxPath, FILE_READ);
    if (!f) {
        Serial.println("Error: failed to open index file");
        return (size_t)-1;
    }

    if (f.size() == 0) {
        f.close();
        Serial.println("Info: index file empty, using 0");
        return 0;
    }

    String s = f.readStringUntil('\n');
    f.close();

    if (s.length() == 0) {
        Serial.println("Error: empty line in index file");
        return (size_t)-1;
    }

    long val = s.toInt();

    if (val == 0 && s != "0") {
        Serial.print("Error: invalid numeric content in index file: \"");
        Serial.print(s);
        Serial.println("\"");
        return (size_t)-1;
    }

    return (size_t)val;
}

void writeLastSent(const char *idxDir, const char *idxFile, size_t NumLines) { 
    String idxPath = String(idxDir) + idxFile;

    File f = SD.open(idxPath, FILE_WRITE | O_TRUNC);  
    if (!f) {
        Serial.println("Failed to write index file");
        return;
    }
    f.println(NumLines);  
    f.flush();      
    f.close();
}
