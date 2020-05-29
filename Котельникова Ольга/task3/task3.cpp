#include <iostream>
#include <conio.h>
using namespace std;
class Temp
{
	int TEMP;
public:
	//конструктор
	Temp(){ TEMP = 25; }
	//функция показа на экран
	void TempFunction(){
		cout << "TEMP = " << TEMP << "\n\n";
	}
	//функция установки значения
	void TempSet(int T){
		TEMP = T;
	}
};
// Класс, реализующий умный указатель
class SmartPointer
{
	// Инкапсулированный указатель
	Temp*ptr;
	//счётчик копий 
	int count_copy;

public:
	//конструктор
	SmartPointer(Temp*p = NULL){
		//записываем 0 при создании копий нет
		count_copy = 0;
		ptr = p;
	}
	// конструктор копирования
	SmartPointer(const SmartPointer&obj){
		//создается копия - увеличиваем счётчик
		ptr = obj.ptr;
		count_copy++;
	}
	//Get- Получить объект
	Temp *Get(){
		return ptr;
	}

	//Set - Установить объект
	void Set(Temp*p = NULL){
		if (ptr==p) count_copy++;
		else{
			count_copy = 0;
			ptr = p;
		}

	}

	void Print(){
		cout << "Object: "; ptr->TempFunction();cout << endl;
		cout << "Links: " << count_copy << endl;

	}

	//Release - Освободить указатель от владения объектом
	void Release(){
		ptr = NULL;
		count_copy = 0;
	}

	//перегрузка оператора равно
	SmartPointer operator=(const SmartPointer&obj){
		//создается копия - увеличиваем счётчик
		ptr = obj.ptr;
		ptr = obj.ptr;
		count_copy++;
		//возвращаем сам объект для ситуации a=b=c
		return *this;
	}

	//перегрузка селектора
	Temp* operator->(){
		return ptr;
	}

	SmartPointer operator*( Temp*p){
		if (this->ptr == p	) count_copy++;
		else{
			count_copy = 0;
			ptr = p;
		}

		
		return *this;
	}

	// уничтожение объекта
	~SmartPointer(){
		//если объект есть и копий нет
		if (ptr != NULL&&count_copy == 0){
			cout << "\n~Delete Object\n";
			//уничтожаем объект
			delete[]ptr;
		}
		//в противном случае(уничтожается копия)
		else{
			//уменьшаем счётчик
			count_copy--;
			cout << "\n~Delete Copy\n";
		}

	}


};

int main(){
	//создаем объект
	Temp*main_ptr = new Temp;
	//инициализируем этим объектом умный указатель
	SmartPointer PTR(main_ptr);
	//проверяем работу умного указателя
	PTR->TempSet(100);
	PTR->TempFunction();
	// создаем копию (работа конструктора копирования)
	SmartPointer PTR2 = PTR;

	for (int i = 0; i < 10;i++)
	PTR2.Set(main_ptr);
	PTR2.Print();
	
	cout << "Get: "; PTR2.Get()->TempFunction();



	_getch();
}
