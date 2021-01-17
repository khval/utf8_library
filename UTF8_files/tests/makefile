
VER=-DVER=6

files =	save_utf8_c.exe  \
		utf8_test_C.exe utf8_upper_lower_test_C.exe \
		utf8_substring_test_C.exe utf8_indexof_test_C.exe \
		utf8_inline_modify_test_C.exe \
		utf8_printf_test_C.exe \
		utf8_left_right_test_C.exe \
		utf8_trim_test_C.exe \
		utf8_merge_test_C.exe \
		utf8_remove_test_C.exe \
		utf8_inline_test_C.exe \
		utf8_insert_test_C.exe \
		utf8_cmp_test_C.exe

notWorking = 

cpp_files = save_utf8_C++.exe

inc = -I../include -I./

all:		$(files)

save_utf8_c.exe: save_utf8_C.c
		gcc $(VER) $(inc) save_utf8_C.c -o save_utf8_c.exe	 

save_utf8_C++.exe: save_utf8_C++.cpp
		c++ save_utf8_C++.cpp -o save_utf8_C++.exe 	

utf8_test_C.exe: utf8_test_C.c
		gcc $(VER) $(inc) utf8_test_C.c -o utf8_test_C.exe 	

utf8_upper_lower_test_C.exe: utf8_upper_lower_test_C.c
		gcc $(VER) $(inc) utf8_upper_lower_test_C.c -o utf8_upper_lower_test_C.exe 	

utf8_substring_test_C.exe: utf8_substring_test_C.c
		gcc $(VER) $(inc) utf8_substring_test_C.c -o utf8_substring_test_C.exe 	

utf8_indexof_test_C.exe: utf8_indexof_test_C.c
		gcc $(VER) $(inc) utf8_indexof_test_C.c -o utf8_indexof_test_C.exe 	

utf8_printf_test_C.exe: utf8_printf_test_C.c
		gcc $(VER) $(inc) utf8_printf_test_C.c -o utf8_printf_test_C.exe 	

utf8_left_right_test_C.exe: utf8_left_right_test_C.c
		gcc $(VER) $(inc) utf8_left_right_test_C.c -o utf8_left_right_test_C.exe 	

utf8_merge_test_C.exe: utf8_merge_test_C.c
		gcc $(VER) $(inc) utf8_merge_test_C.c -o utf8_merge_test_C.exe 	

utf8_remove_test_C.exe: utf8_remove_test_C.c
		gcc $(VER) $(inc) utf8_remove_test_C.c -o utf8_remove_test_C.exe 	

utf8_insert_test_C.exe: utf8_insert_test_C.c
		gcc $(VER) $(inc) utf8_insert_test_C.c -o utf8_insert_test_C.exe 	

utf8_trim_test_C.exe: utf8_trim_test_C.c
		gcc $(VER) $(inc) utf8_trim_test_C.c -o utf8_trim_test_C.exe 	

utf8_inline_test_C.exe: utf8_inline_test_C.c
		gcc $(VER) $(inc) utf8_inline_test_C.c -o utf8_inline_test_C.exe 	

utf8_inline_modify_test_C.exe: utf8_inline_modify_test_C.c
		gcc $(VER) $(inc) utf8_inline_modify_test_C.c -o utf8_inline_modify_test_C.exe 	

utf8_cmp_test_C.exe: utf8_cmp_test_C.c
		gcc $(VER) $(inc) utf8_cmp_test_C.c -o utf8_cmp_test_C.exe 	

clean:
		delete #?.exe
