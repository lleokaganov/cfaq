if(CMD_EQ("upgrade")) { if(!ISNET) return EMPTY_STRING; // если нет подключения к местной сети
#ifdef USE_UDP
    if(PARG(s,1) == "udp") {
        LOGI(LOG_UPGRADE,"Upgrade: "+OKER(UpgradeALLudp()) );
        return EMPTY_STRING;
    }
#endif
LOGI(LOG_UPGRADE,"Upgrade: "+OKER(UpgradeALL()) );
return EMPTY_STRING;
}

if(CMD_EQ("upgrade.firmware")) { if(!ISNET) return EMPTY_STRING; // если нет подключения к местной сети
    String url=PARG_OTHER(s,0); if(url=="") url="http://lleo.me/ESP8266/index.php?firmware";
    LOGI(LOG_UPGRADE,"Upgrade firmware: "+String(url));
    upgrade_url(url);
    return EMPTY_STRING;
}
