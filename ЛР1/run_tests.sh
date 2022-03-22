# Autotests for programs main.sh and main.c

./main.sh ~/lab1 >> sh_tests.txt
echo -e "\n\n" >> sh_tests.txt

./main.sh ~/lab1/dir1/dir3 >> sh_tests.txt
echo -e "\n\n" >> sh_tests.txt

./main.sh ~/lab1/dir2 >> sh_tests.txt
echo -e "\n\n" >> sh_tests.txt

./main.sh aboba >> sh_tests.txt
echo -e "\n\n" >> sh_tests.txt

./main.sh aboba1 aboba2 >> sh_tests.txt
echo -e "\n\n" >> sh_tests.txt

./main.sh >> sh_tests.txt
echo -e "\n\n" >> sh_tests.txt



./main ~/lab1 >> c_tests.txt
echo -e "\n\n" >> c_tests.txt

./main ~/lab1/dir1/dir3 >> c_tests.txt
echo -e "\n\n" >> c_tests.txt

./main ~/lab1/dir2 >> c_tests.txt
echo -e "\n\n" >> c_tests.txt

./main aboba >> c_tests.txt
echo -e "\n\n" >> c_tests.txt

./main aboba1 aboba2 >> c_tests.txt
echo -e "\n\n" >> c_tests.txt

./main >> c_tests.txt
echo -e "\n\n" >> c_tests.txt
