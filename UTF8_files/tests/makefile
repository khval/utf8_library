
VER=-DVER=6

files =	save_utf8_c.exe  \
		utf8_test_C.exe utf8_upper_lower_test_C.exe \
		utf8_substring_test_C.exe utf8_indexof_test_C.exe \
		utf8_printf_test_C.exe utf8_left_right_test_C.exe \
		utf8_merge_test_C.exe utf8_trim_test_C.exe \
		utf8_insert_test_C.exe utf8_remove_test_C.exe \
		utf8_inline_test_C.exe utf8_inline_modify_test_C.exe \
		utf8_cmp_test_C.exe

cpp_files = save_utf8_C++.exe

all:		$(files)

save_utf8_c.exe: save_utf8_C.c
		gcc $(VER) save_utf8_C.c -o save_utf8_c.exe	-I../include

save_utf8_C++.exe: save_utf8_C++.cpp
		c++ save_utf8_C++.cpp -o save_utf8_C++.exe 	-I../include

utf8_test_C.exe: utf8_test_C.c
		gcc $(VER) utf8_test_C.c -o utf8_test_C.exe 	-I../include

utf8_upper_lower_test_C.exe: utf8_upper_lower_test_C.c
		gcc $(VER) utf8_upper_lower_test_C.c -o utf8_upper_lower_test_C.exe 	-I../include

utf8_substring_test_C.exe: utf8_substring_test_C.c
		gcc $(VER) utf8_substring_test_C.c -o utf8_substring_test_C.exe 	-I../include

utf8_indexof_test_C.exe: utf8_indexof_test_C.c
		gcc $(VER) utf8_indexof_test_C.c -o utf8_indexof_test_C.exe 	-I../include

utf8_printf_test_C.exe: utf8_printf_test_C.c
		gcc $(VER) utf8_printf_test_C.c -o utf8_printf_test_C.exe 	-I../include

utf8_left_right_test_C.exe: utf8_left_right_test_C.c
		gcc $(VER) utf8_left_right_test_C.c -o utf8_left_right_test_C.exe 	-I../include

utf8_merge_test_C.exe: utf8_merge_test_C.c
		gcc $(VER) utf8_merge_test_C.c -o utf8_merge_test_C.exe 	-I../include

utf8_remove_test_C.exe: utf8_remove_test_C.c
		gcc $(VER) utf8_remove_test_C.c -o utf8_remove_test_C.exe 	-I../include

utf8_insert_test_C.exe: utf8_insert_test_C.c
		gcc $(VER) utf8_insert_test_C.c -o utf8_insert_test_C.exe 	-I../include

utf8_trim_test_C.exe: utf8_trim_test_C.c
		gcc $(VER) utf8_trim_test_C.c -o utf8_trim_test_C.exe 	-I../include

utf8_inline_test_C.exe: utf8_inline_test_C.c
		gcc $(VER) utf8_inline_test_C.c -o utf8_inline_test_C.exe 	-I../include

utf8_inline_modify_test_C.exe: utf8_inline_modify_test_C.c
		gcc $(VER) utf8_inline_modify_test_C.c -o utf8_inline_modify_test_C.exe 	-I../include

utf8_cmp_test_C.exe: utf8_cmp_test_C.c
		gcc $(VER) utf8_cmp_test_C.c -o utf8_cmp_test_C.exe 	-I../include

clean:
		delete #?.exe
