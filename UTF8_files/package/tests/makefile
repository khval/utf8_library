
VER=-DVER=6

INC = -I../../include -I./

files =	save_utf8_C.exe  \
		utf8_test_C.exe utf8_upper_lower_test_C.exe \
		utf8_substring_test_C.exe  \
		utf8_IndexOf_test_C.exe \
		utf8_inline_modify_test_C.exe \
		utf8_printf_test_C.exe \
		utf8_left_right_test_C.exe \
		utf8_Trim_test_C.exe \
		utf8_Merge_test_C.exe \
		utf8_Remove_test_C.exe \
		utf8_inline_test_C.exe \
		utf8_Insert_test_C.exe \
		utf8_cmp_test_C.exe

cpp_files = save_utf8_C++.exe


all:		$(files)

%.exe:	%.c
		gcc  -Wall -O3  $(INC) $(DEBUG) $(VER)  -D__USE_INLINE__ -D__no_stdlib__ $(@:.exe=.c) -o $@

clean:
		delete #?.exe
