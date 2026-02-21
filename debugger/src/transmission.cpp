#include "transmission.h"

const char* SMTP_HOST       = "smtp.gmail.com";
const int   SMTP_PORT       = 465;
const char* AUTHOR_EMAIL    = "********@gmail.com";
const char* AUTHOR_PASSWORD = "**** **** **** ****"; 
const char* RECIPIENT_EMAIL = "********@gmail.com";

SMTPSession smtp;

void smtpCallback(SMTP_Status status) {
    Serial.println(status.info());
}

bool Send_All_Data_Email(int Soc,int Volt) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Error: WiFi not connected!");
        return false;
    }

    smtp.debug(1); 
    smtp.callback(smtpCallback);

    ESP_Mail_Session session;
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "gmail.com";

    SMTP_Message message;
    message.sender.name = "ESP32 Data";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ESP32 Sensor Dataset";
    message.addRecipient("User", RECIPIENT_EMAIL);

    String emailBody = "Battery: " + String(Soc) + "% (" + String(Volt/100.0, 2) + "V)\n";
    emailBody += "Data Logs Attached.\n";
    message.text.content = emailBody;

    SMTP_Attachment *att1 = nullptr;
    SMTP_Attachment *att2 = nullptr;

    String lcPath_temp = String(DIR_TEMP) + "/lc_temp.csv";
    if (SD.exists(lcPath_temp)) {
        att1 = new SMTP_Attachment();
        att1->descr.filename = "LoadCell_Data.csv";
        att1->descr.mime = "text/csv"; 
        att1->file.path = lcPath_temp.c_str();
        att1->file.storage_type = esp_mail_file_storage_type_sd; 
        message.addAttachment(*att1);
    }

    String shtPath_temp = String(DIR_TEMP) + "/sht_temp.csv";
    if (SD.exists(shtPath_temp)) {
        att2 = new SMTP_Attachment();
        att2->descr.filename = "SHT45_Data.csv";
        att2->descr.mime = "text/csv";
        att2->file.path = shtPath_temp.c_str();
        att2->file.storage_type = esp_mail_file_storage_type_sd;
        message.addAttachment(*att2);
    }

    if (!smtp.connect(&session)) {
        Serial.println("SMTP Connect Failed");
        if (att1) delete att1;
        if (att2) delete att2;
        return false;
    }

    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("Error sending Email: " + smtp.errorReason());
        if (att1) delete att1;
        if (att2) delete att2;
        smtp.closeSession();
        return false;
    }

    File f1 = SD.open(lcPath_temp, FILE_WRITE | O_TRUNC);
    if (f1) {
        f1.close();
        Serial.printf("Cleared: %s\n", lcPath.c_str());
    } else {
        Serial.printf("Clear LC failed: %s\n", lcPath.c_str());
    }

    File f2 = SD.open(shtPath_temp, FILE_WRITE | O_TRUNC);
    if (f2) {
        f2.close();
        Serial.printf("Cleared: %s\n", shtPath.c_str());
    } else {
        Serial.printf("Clear SHT failed: %s\n", shtPath.c_str());
    }

    if (att1) delete att1;
    if (att2) delete att2;
    smtp.closeSession();

    Serial.println("Email Sent Successfully!");
    return true;
}




