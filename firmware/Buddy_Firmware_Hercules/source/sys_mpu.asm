;-------------------------------------------------------------------------------
; sys_mpu.asm
;
; (c) Texas Instruments 2009-2013, All rights reserved.
;

    .text
    .arm


;-------------------------------------------------------------------------------
; Initalize Mpu
; SourceId : MPU_SourceId_001
; DesignId : MPU_DesignId_001
; Requirements : HL_SR487

    .def     _mpuInit_
    .asmfunc

_mpuInit_
        stmfd sp!, {r0}
        ; Disable mpu
        mrc   p15, #0, r0, c1, c0, #0
        bic   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ; Disable background region
        mrc   p15, #0, r0,      c1, c0, #0
        bic   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ; Setup region 1
        mov   r0,  #0
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r1Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0008
        orr   r0,  r0,    #0x1000
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((1 << 15) + (1 << 14) + (1 << 13) + (1 << 12) + (1 << 11) + (1 << 10) + (1 <<  9) + (1 <<  8) + (0x1F << 1) + (1)) 
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region 2
        mov   r0,  #1
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r2Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0008
        orr   r0,  r0,    #0x0600
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((0 << 15) + (0 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x15 << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region  
        mov   r0,  #2
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r3Base
        mcr   p15, #0,    r0, c6, c1, #0    
        mov   r0,  #0x0008
        orr   r0,  r0,    #0x0300
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((0 << 15) + (0 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x11 << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region 4
        mov   r0,  #3
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r4Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0008
        orr   r0,  r0,    #0x0300
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((0 << 15) + (0 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x11 << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region 5
        mov   r0,  #4
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r5Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0000
        orr   r0,  r0,    #0x0300
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((1 << 15) + (1 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x19 << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region 6
        mov   r0,  #5
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r6Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0000
        orr   r0,  r0,    #0x0300
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((0 << 15) + (0 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x1A << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region 7
        mov   r0,  #6
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r7Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0008
        orr   r0,  r0,    #0x1200
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((0 << 15) + (0 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x16 << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2
        ; Setup region 8
        mov   r0,  #7
        mcr   p15, #0,    r0, c6, c2, #0
        ldr   r0,  r8Base
        mcr   p15, #0,    r0, c6, c1, #0
        mov   r0,  #0x0010
        orr   r0,  r0,    #0x1300
        mcr   p15, #0,    r0, c6, c1, #4
        movw  r0,  #((0 << 15) + (0 << 14) + (0 << 13) + (0 << 12) + (0 << 11) + (0 << 10) + (0 <<  9) + (0 <<  8) + (0x17 << 1) + (1))
        mcr   p15, #0,    r0, c6, c1, #2

        ; Enable mpu background region
        mrc   p15, #0, r0,      c1, c0, #0
        orr   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ; Enable mpu
        mrc   p15, #0, r0, c1, c0, #0
        orr   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}
        bx    lr

r1Base  .word 0x00000000  
r2Base  .word 0x00000000  
r3Base  .word 0x08000000  
r4Base  .word 0x08400000  
r5Base  .word 0x60000000  
r6Base  .word 0x80000000  
r7Base  .word 0xF0000000  
r8Base  .word 0xFC000000  


    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Mpu
; SourceId : MPU_SourceId_002
; DesignId : MPU_DesignId_002
; Requirements : HL_SR488

    .def     _mpuEnable_
    .asmfunc

_mpuEnable_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c1, c0, #0
        orr   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Mpu
; SourceId : MPU_SourceId_003
; DesignId : MPU_DesignId_003
; Requirements : HL_SR488

    .def     _mpuDisable_
    .asmfunc

_mpuDisable_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c1, c0, #0
        bic   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Mpu background region
; SourceId : MPU_SourceId_004
; DesignId : MPU_DesignId_004
; Requirements : HL_SR488

    .def     _mpuEnableBackgroundRegion_
    .asmfunc

_mpuEnableBackgroundRegion_

        stmfd sp!, {r0}
        mrc   p15, #0, r0,      c1, c0, #0
        orr   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ldmfd sp!, {r0}		
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Mpu background region
; SourceId : MPU_SourceId_005
; DesignId : MPU_DesignId_005
; Requirements : HL_SR488

    .def     _mpuDisableBackgroundRegion_
    .asmfunc

_mpuDisableBackgroundRegion_

        stmfd sp!, {r0}
        mrc   p15, #0, r0,      c1, c0, #0
        bic   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Returns number of implemented Mpu regions
; SourceId : MPU_SourceId_006
; DesignId : MPU_DesignId_006
; Requirements : HL_SR490

    .def     _mpuGetNumberOfRegions_
    .asmfunc

_mpuGetNumberOfRegions_

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0, ROR #8
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Returns the type of the implemented mpu regions
; SourceId : MPU_SourceId_007
; DesignId : MPU_DesignId_007
; Requirements : HL_SR490

    .def     _mpuAreRegionsSeparate_
    .asmfunc

_mpuAreRegionsSeparate_

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set mpu region number
; SourceId : MPU_SourceId_008
; DesignId : MPU_DesignId_008
; Requirements : HL_SR489

    .def     _mpuSetRegion_
    .asmfunc

_mpuSetRegion_

        mcr   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get mpu region number
; SourceId : MPU_SourceId_009
; DesignId : MPU_DesignId_009
; Requirements : HL_SR490

    .def     _mpuGetRegion_
    .asmfunc

_mpuGetRegion_

        mrc   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set base address
; SourceId : MPU_SourceId_010
; DesignId : MPU_DesignId_010
; Requirements : HL_SR489

    .def     _mpuSetRegionBaseAddress_
    .asmfunc

_mpuSetRegionBaseAddress_

        mcr   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get base address
; SourceId : MPU_SourceId_011
; DesignId : MPU_DesignId_011
; Requirements : HL_SR490

    .def     _mpuGetRegionBaseAddress_
    .asmfunc

_mpuGetRegionBaseAddress_

        mrc   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set type and permission
; SourceId : MPU_SourceId_012
; DesignId : MPU_DesignId_012
; Requirements : HL_SR489

    .def     _mpuSetRegionTypeAndPermission_
    .asmfunc

_mpuSetRegionTypeAndPermission_

        orr   r0,  r0, r1
        mcr   p15, #0, r0, c6, c1, #4
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get type
; SourceId : MPU_SourceId_013
; DesignId : MPU_DesignId_013
; Requirements : HL_SR490

    .def     _mpuGetRegionType_
    .asmfunc

_mpuGetRegionType_

        mrc   p15, #0, r0,     c6, c1, #4
        bic   r0,  r0, #0xFF00
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get permission
; SourceId : MPU_SourceId_014
; DesignId : MPU_DesignId_014
; Requirements : HL_SR490

    .def     _mpuGetRegionPermission_
    .asmfunc

_mpuGetRegionPermission_

        mrc   p15, #0, r0,   c6, c1, #4
        bic   r0,  r0, #0xFF
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set region size register value
; SourceId : MPU_SourceId_015
; DesignId : MPU_DesignId_015
; Requirements : HL_SR489

    .def     _mpuSetRegionSizeRegister_
    .asmfunc

_mpuSetRegionSizeRegister_


        mcr   p15, #0, r0, c6, c1, #2
        bx    lr

    .endasmfunc

    

;-------------------------------------------------------------------------------

