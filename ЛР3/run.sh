make program
echo -e "\n"

echo -e "--------------------- ТЕСТ №1 ---------------------"
./main 1
echo -e "\n"

echo -e "--------------------- ТЕСТ №2 ---------------------"
./main 5
echo -e "\n"

echo -e "--------------------- ТЕСТ №3 ---------------------"
./main
echo -e "\n"

echo -e "--------------------- ТЕСТ №4 ---------------------"
./main 6
echo -e "\n"

echo -e "--------------------- ТЕСТ №5 ---------------------"
./main -1
echo -e "\n"

echo -e "--------------------- ТЕСТ №6 ---------------------"
rm process
./main 3
echo -e "\n"