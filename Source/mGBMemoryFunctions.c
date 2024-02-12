// Define the size of the SRAM
#define SRAM_SIZE 8192 // 8 KB

// Define an SRAM flag byte to indicate if SRAM has been initialized
#define SRAM_INITIALIZED_BYTE 512
#define SRAM_INITIALIZED_FLAG 0xF7

// Modify the saveData array size to accommodate SRAM
UBYTE saveData[SRAM_SIZE];

void saveDataSet(UBYTE synth) {
    ENABLE_RAM_MBC1;
    x = (synth + 24U);
    x = dataSet[x] * 8;
    i = 0;
    switch(synth) {
        case 0:
            for(j = 0; j != 7; j++) {
                saveData[x + i] = dataSet[j];
                i++;
            }
            break;
        case 1:
            for(j = 7; j != 13; j++) {
                saveData[128U + x + i] = dataSet[j];
                i++;
            }
            break;
        case 2:
            for(j = 13; j != 20; j++) {
                saveData[256U + x + i] = dataSet[j];
                i++;
            }
            break;
        case 3:
            for(j = 20; j != 24; j++) {
                saveData[384U + x + i] = dataSet[j];
                i++;
            }
            break;
    }
    DISABLE_RAM_MBC1;
}

void loadDataSet(UBYTE synth) {
    ENABLE_RAM_MBC1;
    x = dataSet[(synth + 24U)] * 8U;
    i = 0;
    switch(synth) {
        case 0U:
            for(j = 0; j != 7; j++) {
                if(!parameterLock[j]) dataSet[j] = saveData[x + i];
                i++;
            }
            break;
        case 1U:
            for(j = 7; j != 13; j++) {
                if(!parameterLock[j]) dataSet[j] = saveData[128U + x + i];
                i++;
            }
            break;
        case 2U:
            for(j = 13; j != 20; j++) {
                if(!parameterLock[j]) dataSet[j] = saveData[256U + x + i];
                i++;
            }
            break;
        case 3U:
            j = 20;
            for(j = 20; j != 24; j++) {
                if(!parameterLock[j]) dataSet[j] = saveData[384U + x + i];
                i++;
            }
            break;
    }
    DISABLE_RAM_MBC1;
}

void snapRecall() {
    if(cursorRowMain == 0x08U) {
        for(l = 0; l < 4; l++) {
            if(cursorEnable[l]) {
                if(!recallMode) {
                    saveDataSet(l);
                } else {
                    loadDataSet(l);
                    updateSynth(l);
                }
            }
        }
    } else {
        if(!recallMode) {
            for(j = 0; j != 24; j++) dataSetSnap[j] = dataSet[j];
        } else {
            for(j = 0; j != 24; j++) dataSet[j] = dataSetSnap[j];
            updateDisplay();
        }
    }
}

void checkMemory() {
    ENABLE_RAM_MBC1;
    if(saveData[SRAM_INITIALIZED_BYTE] != SRAM_INITIALIZED_FLAG) {
        for(x = 0; x != SRAM_SIZE; x += 512) {
            l = 0;
            for(i = 0; i < 7; i++) {
                saveData[(x + l)] = dataSet[i];
                l++;
            }
            l = 0;
            for(i = 7; i != 13; i++) {
                saveData[(x + 128U + l)] = dataSet[i];
                l++;
            }
            l = 0;
            for(i = 13; i != 20; i++) {
                saveData[(x + 256U + l)] = dataSet[i];
                l++;
            }
            l = 0;
            for(i = 20; i != 24; i++) {
                saveData[(x + 384U + l)] = dataSet[i];
                l++;
            }
        }
        saveData[SRAM_INITIALIZED_BYTE] = SRAM_INITIALIZED_FLAG;
    }
    DISABLE_RAM_MBC1;
    
    for(j = 0; j != 24; j++) dataSetSnap[j] = dataSet[j];
}
