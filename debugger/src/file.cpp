#include "file.h" 

size_t BootCount(const char *idxDir, const char *idxFile) {
    char idxPath[64];
    snprintf(idxPath, sizeof(idxPath), "%s%s", idxDir, idxFile);

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

    char line[32];
    size_t len = f.readBytesUntil('\n', line, sizeof(line)-1);
    line[len] = '\0';  
    f.close();

    if (len == 0) {
        Serial.println("Error: empty line in index file");
        return (size_t)-1;
    }

    long val = atol(line);  

    if (val == 0 && line[0] != '0') {
        Serial.print("Error: invalid numeric: \"");
        Serial.print(line);
        Serial.println("\"");
        return (size_t)-1;
    }

    return (size_t)val;
}

void writeLastSent(const char *idxDir, const char *idxFile, size_t NumLines) { 
    char idxPath[64];
    snprintf(idxPath, sizeof(idxPath), "%s%s", idxDir, idxFile);
    File f = SD.open(idxPath, FILE_WRITE);
    if (!f) {
        Serial.println("Failed to write index file");
        return;
    }
    char buf[16];
    snprintf(buf, sizeof(buf), "%zu\n", NumLines);
    f.print(buf);
    f.flush();      
    f.close();
}
