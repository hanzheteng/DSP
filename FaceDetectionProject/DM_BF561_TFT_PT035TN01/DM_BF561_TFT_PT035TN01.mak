# Generated by the VisualDSP++ IDDE

# Note:  Any changes made to this Makefile will be lost the next time the
# matching project file is loaded into the IDDE.  If you wish to preserve
# changes, rename this file and run it externally to the IDDE.

# The syntax of this Makefile is such that GNU Make v3.77 or higher is
# required.

# The current working directory should be the directory in which this
# Makefile resides.

# Supported targets:
#     DM_BF561_TFT_PT035TNO1_Debug
#     DM_BF561_TFT_PT035TNO1_Debug_clean

# Define this variable if you wish to run this Makefile on a host
# other than the host that created it and VisualDSP++ may be installed
# in a different directory.

ADI_DSP=D:\Program Files\Analog Devices\VisualDSP 5.0


# $VDSP is a gmake-friendly version of ADI_DIR

empty:=
space:= $(empty) $(empty)
VDSP_INTERMEDIATE=$(subst \,/,$(ADI_DSP))
VDSP=$(subst $(space),\$(space),$(VDSP_INTERMEDIATE))

RM=cmd /C del /F /Q

#
# Begin "DM_BF561_TFT_PT035TNO1_Debug" configuration
#

ifeq ($(MAKECMDGOALS),DM_BF561_TFT_PT035TNO1_Debug)

DM_BF561_TFT_PT035TNO1_Debug : ./Debug/DM_BF561_TFT_PT035TNO1.dxe 

Debug/dm_cpu.doj :../dm_cpu.c $(VDSP)/Blackfin/include/ccblkfn.h $(VDSP)/Blackfin/include/stdlib.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/stdlib_bf.h $(VDSP)/Blackfin/include/builtins.h $(VDSP)/Blackfin/include/sys/builtins_support.h $(VDSP)/Blackfin/include/fract_typedef.h $(VDSP)/Blackfin/include/fr2x16_typedef.h $(VDSP)/Blackfin/include/r2x16_typedef.h $(VDSP)/Blackfin/include/raw_typedef.h $(VDSP)/Blackfin/include/sys/anomaly_macros_rtl.h $(VDSP)/Blackfin/include/sys/mc_typedef.h $(VDSP)/Blackfin/include/cdefBF561.h $(VDSP)/Blackfin/include/defBF561.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/sys/exception.h ../dm_bf5xx.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/cdefBF53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h ../dm_types.h ../dm_cpu.h 
	@echo "..\dm_cpu.c"
	$(VDSP)/ccblkfn.exe -c ..\dm_cpu.c -file-attr ProjectName=DM_BF561_TFT_PT035TNO1 -g -structs-do-not-overlap -no-multiline -I ../ -double-size-32 -decls-strong -w -si-revision 0.5 -proc ADSP-BF561 -o .\Debug\dm_cpu.doj -MM

Debug/dm_tft_ppiinit.doj :dm_tft_ppiinit.c ../dm_bf5xx.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/cdefBF53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/cdefBF561.h $(VDSP)/Blackfin/include/defBF561.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/sys/exception.h ../dm_types.h 
	@echo ".\dm_tft_ppiinit.c"
	$(VDSP)/ccblkfn.exe -c .\dm_tft_ppiinit.c -file-attr ProjectName=DM_BF561_TFT_PT035TNO1 -g -structs-do-not-overlap -no-multiline -I ../ -double-size-32 -decls-strong -w -si-revision 0.5 -proc ADSP-BF561 -o .\Debug\dm_tft_ppiinit.doj -MM

Debug/dm_tft_pt035nt01init.doj :dm_tft_pt035nt01init.c ../dm_bf5xx.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/cdefBF53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/cdefBF561.h $(VDSP)/Blackfin/include/defBF561.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/sys/exception.h ../dm_types.h 
	@echo ".\dm_tft_pt035nt01init.c"
	$(VDSP)/ccblkfn.exe -c .\dm_tft_pt035nt01init.c -file-attr ProjectName=DM_BF561_TFT_PT035TNO1 -g -structs-do-not-overlap -no-multiline -I ../ -double-size-32 -decls-strong -w -si-revision 0.5 -proc ADSP-BF561 -o .\Debug\dm_tft_pt035nt01init.doj -MM

Debug/dm_tft_spi.doj :dm_tft_spi.c $(VDSP)/Blackfin/include/cdefBF561.h $(VDSP)/Blackfin/include/defBF561.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h ../dm_bf5xx.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/cdefBF53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/sys/exception.h ../dm_types.h 
	@echo ".\dm_tft_spi.c"
	$(VDSP)/ccblkfn.exe -c .\dm_tft_spi.c -file-attr ProjectName=DM_BF561_TFT_PT035TNO1 -g -structs-do-not-overlap -no-multiline -I ../ -double-size-32 -decls-strong -w -si-revision 0.5 -proc ADSP-BF561 -o .\Debug\dm_tft_spi.doj -MM

Debug/main.doj :main.C ../dm_types.h ../dm_bf5xx.h $(VDSP)/Blackfin/include/stdio.h $(VDSP)/Blackfin/include/yvals.h $(VDSP)/Blackfin/include/cdefBF53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h $(VDSP)/Blackfin/include/cdefBF561.h $(VDSP)/Blackfin/include/defBF561.h $(VDSP)/Blackfin/include/def_LPBlackfin.h $(VDSP)/Blackfin/include/cdef_LPBlackfin.h $(VDSP)/Blackfin/include/sys/exception.h 2.dat 
	@echo ".\main.C"
	$(VDSP)/ccblkfn.exe -c .\main.C -file-attr ProjectName=DM_BF561_TFT_PT035TNO1 -g -structs-do-not-overlap -no-multiline -I ../ -double-size-32 -decls-strong -w -si-revision 0.5 -proc ADSP-BF561 -o .\Debug\main.doj -MM

./Debug/DM_BF561_TFT_PT035TNO1.dxe :./adsp-BF561.ldf $(VDSP)/Blackfin/lib/bf561_rev_0.5/crtsf561y.doj ./Debug/dm_cpu.doj ./Debug/dm_tft_ppiinit.doj ./Debug/dm_tft_pt035nt01init.doj ./Debug/dm_tft_spi.doj ./Debug/main.doj $(VDSP)/Blackfin/lib/cplbtab561a.doj $(VDSP)/Blackfin/lib/bf561_rev_0.5/libsmall561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/__initsbsz561.doj $(VDSP)/Blackfin/lib/bf561_rev_0.5/libio561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libc561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libm3free561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libevent561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libx561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libcpp561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libcpprt561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libf64ieee561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libdsp561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libsftflt561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/libetsi561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/Debug/libssl561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/Debug/libdrv561y.dlb $(VDSP)/Blackfin/lib/bf561_rev_0.5/idle561y.doj $(VDSP)/Blackfin/lib/bf561_rev_0.5/librt_fileio561y.dlb 
	@echo "Linking..."
	$(VDSP)/ccblkfn.exe .\Debug\dm_cpu.doj .\Debug\dm_tft_ppiinit.doj .\Debug\dm_tft_pt035nt01init.doj .\Debug\dm_tft_spi.doj .\Debug\main.doj -T .\adsp-BF561.ldf -L .\Debug -add-debug-libpaths -flags-link -od,.\Debug -o .\Debug\DM_BF561_TFT_PT035TNO1.dxe -proc ADSP-BF561 -si-revision 0.5 -flags-link -MM

endif

ifeq ($(MAKECMDGOALS),DM_BF561_TFT_PT035TNO1_Debug_clean)

DM_BF561_TFT_PT035TNO1_Debug_clean:
	-$(RM) "Debug\dm_cpu.doj"
	-$(RM) "Debug\dm_tft_ppiinit.doj"
	-$(RM) "Debug\dm_tft_pt035nt01init.doj"
	-$(RM) "Debug\dm_tft_spi.doj"
	-$(RM) "Debug\main.doj"
	-$(RM) ".\Debug\DM_BF561_TFT_PT035TNO1.dxe"
	-$(RM) ".\Debug\*.ipa"
	-$(RM) ".\Debug\*.opa"
	-$(RM) ".\Debug\*.ti"
	-$(RM) ".\Debug\*.pgi"
	-$(RM) ".\*.rbld"

endif


