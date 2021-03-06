var group__i2c__hal =
[
    [ "i2c_status_t", "group__i2c__hal.html#gada1e8e589688e2d6626421d8d85d9c53", [
      [ "kStatus_I2C_Success", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a41b146d605a2dcf3daa92d2f8847f036", null ],
      [ "kStatus_I2C_Initialized", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53ace6396a489bae0015c0629ba8abf2b3f", null ],
      [ "kStatus_I2C_Fail", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a64bbd841ac2d449dd2f8ea725a1407d4", null ],
      [ "kStatus_I2C_Busy", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a49091894b590d7e479605bf113918952", null ],
      [ "kStatus_I2C_Timeout", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a81ad7cc198436cabbe91ea55c5288747", null ],
      [ "kStatus_I2C_ReceivedNak", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a22495915fb9a79a80f89d93561e3769a", null ],
      [ "kStatus_I2C_SlaveTxUnderrun", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a59390c2b988d02e016ec014f9e014557", null ],
      [ "kStatus_I2C_SlaveRxOverrun", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53ab286ded9de770504a53c67c4fa7ee64b", null ],
      [ "kStatus_I2C_AribtrationLost", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a436dd56a766dbe6a7f2bddd7f29463d1", null ],
      [ "kStatus_I2C_StopSignalFail", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a645e05fc502410278210540341111c61", null ],
      [ "kStatus_I2C_Idle", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a628d242f7bc0e3d5949c7f73eafaa508", null ],
      [ "kStatus_I2C_NoReceiveInProgress", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53a779bf6493f125fdbbd840322ca7a3767", null ],
      [ "kStatus_I2C_NoSendInProgress", "group__i2c__hal.html#ggada1e8e589688e2d6626421d8d85d9c53ada96d88b7452662773fbfaea7f2b835d", null ]
    ] ],
    [ "i2c_status_flag_t", "group__i2c__hal.html#ga8d14ab93ed4623e6f160dead9efed688", null ],
    [ "i2c_direction_t", "group__i2c__hal.html#gab49c827b45635206f06e5737606e4611", [
      [ "kI2CReceive", "group__i2c__hal.html#ggab49c827b45635206f06e5737606e4611a452f63617b04cbce09e4a23338c21a4e", null ],
      [ "kI2CSend", "group__i2c__hal.html#ggab49c827b45635206f06e5737606e4611af381933c365d3290c3fceeec2178077a", null ]
    ] ],
    [ "I2C_HAL_Init", "group__i2c__hal.html#gad35451486842f04187c74ab5a2c85d6d", null ],
    [ "I2C_HAL_Enable", "group__i2c__hal.html#ga9e2b7eb9fbb09a8ea6aaa178991cfc2c", null ],
    [ "I2C_HAL_Disable", "group__i2c__hal.html#ga0fb5954898e721a92792cdb83edc23da", null ],
    [ "I2C_HAL_SetGlitchWidth", "group__i2c__hal.html#ga80f4e3524b3c07f1db5a9563dd122808", null ],
    [ "I2C_HAL_SetWakeupCmd", "group__i2c__hal.html#ga13066cad23a79abfa8bda495d64f774c", null ],
    [ "I2C_HAL_SetBaudRate", "group__i2c__hal.html#ga9fac6798511182c60575ca3c81c84141", null ],
    [ "I2C_HAL_SetFreqDiv", "group__i2c__hal.html#gac88dde551848a6979821824b749ae3ff", null ],
    [ "I2C_HAL_SetSlaveBaudCtrlCmd", "group__i2c__hal.html#ga8156dd9d331afa57effd59abfa7abeb2", null ],
    [ "I2C_HAL_SendStart", "group__i2c__hal.html#ga1b3a849cc66e30eaff24a49f06da2a77", null ],
    [ "I2C_HAL_SendStop", "group__i2c__hal.html#ga0f1d30769797349d8b4d389616d4fe22", null ],
    [ "I2C_HAL_SendAck", "group__i2c__hal.html#gaf38e1973ec65a7020037e48c43141d3d", null ],
    [ "I2C_HAL_SendNak", "group__i2c__hal.html#gafeb448cf297107041b29bd00bffa7b7b", null ],
    [ "I2C_HAL_SetDirMode", "group__i2c__hal.html#ga9b91479a9e4574f215277e3159a59393", null ],
    [ "I2C_HAL_GetDirMode", "group__i2c__hal.html#ga6531f579265baeeb726763954e9701d7", null ],
    [ "I2C_HAL_ReadByte", "group__i2c__hal.html#gae0fc24a93fc3c7683ea4556ac4551993", null ],
    [ "I2C_HAL_WriteByte", "group__i2c__hal.html#gab372be865d1346620e4fad6a9e4d9d9c", null ],
    [ "I2C_HAL_ReadByteBlocking", "group__i2c__hal.html#gaf444402934ea83fef7f31d50b4cc9b87", null ],
    [ "I2C_HAL_WriteByteBlocking", "group__i2c__hal.html#ga1788be3f479fe50fdc0a4e10a229c617", null ],
    [ "I2C_HAL_MasterReceiveDataPolling", "group__i2c__hal.html#gabfe975ffe599ff56686c95341b0b68d3", null ],
    [ "I2C_HAL_MasterSendDataPolling", "group__i2c__hal.html#ga78c3201bbfa7c1da9eba4a2e4467c368", null ],
    [ "I2C_HAL_SlaveSendDataPolling", "group__i2c__hal.html#ga62997d497e038f50f3a7c644e627817c", null ],
    [ "I2C_HAL_SlaveReceiveDataPolling", "group__i2c__hal.html#ga9fc129a794e08d320a9adf48104ce55e", null ],
    [ "I2C_HAL_SetAddress7bit", "group__i2c__hal.html#ga1f0c1d284d62c096bcf1361e7bedef89", null ],
    [ "I2C_HAL_SetAddress10bit", "group__i2c__hal.html#ga581c7e046e0c94bf04ab20d1575be2e1", null ],
    [ "I2C_HAL_SetExtensionAddrCmd", "group__i2c__hal.html#gafa1ea08d6239c16430a5e23994ef6007", null ],
    [ "I2C_HAL_GetExtensionAddrCmd", "group__i2c__hal.html#ga461790c1863112dd5254b82decfc03ff", null ],
    [ "I2C_HAL_SetGeneralCallCmd", "group__i2c__hal.html#ga01d4216daebe80676d7ffe5284588d4e", null ],
    [ "I2C_HAL_SetRangeMatchCmd", "group__i2c__hal.html#gabb225d27ea8f70670cd9b8b873cd2c2c", null ],
    [ "I2C_HAL_SetUpperAddress7bit", "group__i2c__hal.html#ga8cc3212490a9f13bf821e0826305a8aa", null ],
    [ "I2C_HAL_GetStatusFlag", "group__i2c__hal.html#ga1d543166df5d5b05f76243cf8254513e", null ],
    [ "I2C_HAL_IsMaster", "group__i2c__hal.html#ga3417857bd772544466a670418754c884", null ],
    [ "I2C_HAL_ClearArbitrationLost", "group__i2c__hal.html#gae59cc22ae7d26559b3073a4a5842c6d7", null ],
    [ "I2C_HAL_SetIntCmd", "group__i2c__hal.html#gabf8a1237bb88713ada97fa9f99d734f4", null ],
    [ "I2C_HAL_GetIntCmd", "group__i2c__hal.html#gaba05d16fc70b55cd9d1ecc1f63b7d66e", null ],
    [ "I2C_HAL_IsIntPending", "group__i2c__hal.html#ga72e315091dacec095425c4b300cb4104", null ],
    [ "I2C_HAL_ClearInt", "group__i2c__hal.html#ga802e162e4d3f3dc425e9baecda0308de", null ]
];