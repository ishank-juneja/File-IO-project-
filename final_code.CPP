#include <iostream.h>
#include <conio.h>
#include <fstream.h>
#include <string.h>
#include <stdio.h>

// Templates

class user  // Primary base class
    {
 public:
  char user_name[50];
  char user_email[50];
};

class viewer : public user
               {
 public:
  long int viewer_code;
};

class admin : public user {
 public:
  long int admin_code;
};
class animal {
 public:
  long int animal_index;  // so as some fancy address like 2016002
  char locations[100];
  char animal_info[100];
  char animal_name[50];
  int population;
  void display_name() {

    cout <<"Index "<<animal_index<<"\n";
    cout <<"Name "<<animal_name<<"\n"<<"\n";
}
  void display_info()  { //Two seperate functions so that in modification ,addition,deletion, if index ,name are manupulated, info is not need to be touched
    cout<<"Locations at which present ";
    cout<<locations<<"\n";
    cout<<"Info about the animal\n";
    cout<<animal_info<<"\n";
    cout<<"Total population "<<population<<"\n"<<"\n";
}
  animal(){}//dummy to prevent error because only copy constructor cant be defined

  animal(animal &copy)  {         //Copy constructor

  animal_index=copy.animal_index;
  strcpy(animal_info, copy.animal_info);
  strcpy(locations, copy.locations);
  strcpy(animal_name, copy.animal_name);
  population=copy.population;
  }

};

// Functions
int animal_list_display() {
  animal obj;
  int flag;
  ifstream fin("animal_list.dat", ios::binary);
  fin.seekg(0);
  cout<<"Current list is\n";
  while (fin)
  {
    fin.read((char*)&obj, sizeof(obj));
    if(fin.eof()) break;  // To prevent double printing

    flag=1;
    obj.display_name();
  }
  fin.seekg(0);
  fin.close();
  return flag;
}
void retrieve_animal()     {
  long int animal_index;
  int flag=0;
  ifstream fin("animal_list.dat", ios::binary);
  animal obj;
  fin.seekg(0);
  cout<<"Enter animal index of your choice\n";//Allows displaying all details of animals with just the input of animal index
  cin>>animal_index;
  while (fin) {

    fin.read((char*)&obj, sizeof(obj));
    if(fin.eof()) break;// To prevent double reading of last record and to ensure correct opening


    if (obj.animal_index == animal_index) {
      flag=1;
      cout << "Animal found\n";
      obj.display_name();
      obj.display_info();
    }
  }
  if(flag==0)
   cout<<"Sorry invalid animal index";
  fin.close();
}

// post viewer login

void post_viewer_login() {
  int flag;
  cout << "Congrats you can now view information of your choice\n";
  flag=animal_list_display();
  if(flag==1)
    retrieve_animal();
  else cout<<"Sorry file is empty\n";
}

void per_change(int old_population, int new_population) {
  float per;
  per = (float(new_population - old_population) / float(old_population)) * 100.0;   //explicit type casting so that a value of 0 isnt returned when we have 1000/5000 !
  cout << "Percent change in animal's population is " << per<<"\n";
}
void modify_animal() {
  long int animal_index_modify;
  int bool=animal_list_display();//returns value of flag , if 1 goes ahead !
  if(bool==1)  {
  cout << "Enter the index of the animal you want to modify \n";
  cin >>animal_index_modify;
  fstream file("animal_list.dat", ios::binary | ios::in | ios::out);
  file.seekg(0);
  animal obj1;
  int flag=0;// flag is used to find the record and bool to find the file
  int op, new_population;
  while(file) {
    long int n=file.tellp();// this is the replacement of (-1*size of obj,ios::cur)
    file.read((char*)&obj1, sizeof(obj1));
    if(file.eof()) break;
    if (obj1.animal_index == animal_index_modify ) {
      animal obj2(obj1);
      flag = 1;
      cout << "Enter\n 1. To modify animal info \n 2. To modify locations \n "<<"3. To modify population \n ";
      cin >> op;
      switch (op) {
	case 1:
	  cout << "Welcome to animal info modification\n";
	  cout<<"Enter new animal info\n";
	  cin.ignore(100, '\n');
	  cin.getline(obj2.animal_info, 50);
	  break;

	case 2:
	  cout << "Welcome to locations modification\n";
	  cout<<"Enter new locations\n";
	  cin.ignore(100, '\n');
	  cin.getline(obj2.locations, 50);
	  break;

	case 3:
	  cout << "Welcome to population modification\n";
	  cout<<"Enter new population\n";
	  cin>>new_population;
	  per_change(obj1.population, new_population);
	  obj2.population=new_population;
	  break;

    default:
          cout << "Sorry invalid choice\n";
      }
      file.seekp(n);
      file.seekg(n);
      file.write((char*)&obj2, sizeof(obj2));

     }
   }
  if (flag == 0)
      cout<<"Sorry invalid animal index\n";
  file.close();
  }
 else
    cout<<"Sorry file is empty\n";
}


void add_animal() {
    fstream fout("animal_list.dat", ios::in | ios::out | ios::binary);
    fout.seekp(0, ios::end);
    animal obj;
    cout << "Enter animal name\n";
    cin.ignore(100, '\n');
    cin.getline(obj.animal_name, 20);
    cout << "Enter animal index\n";
    cin >> obj.animal_index;
    cout << "Enter locations\n";
    cin.ignore(100, '\n');
    cin.getline(obj.locations, 50);
    cout<<"Enter total population\n";
    cin>>obj.population;
    cout << "Enter animal info\n";
    cin.ignore(100, '\n');
    cin.getline(obj.animal_info, 50);
    fout.write((char*)&obj, sizeof(obj));
    fout.close();

}
void delete_animal() {
  long int animal_index;
  int bool, found, result;
  found=0;
  animal obj;
  ifstream fin("animal_list.dat", ios::binary);
  ofstream fout("temp.dat", ios::binary);
  bool=animal_list_display();
  if(bool==1)   {
  cout << "Enter index of animal to be deleted\n";
  cin >>animal_index;
  fin.seekg(0);
  fout.seekp(0);
  while (fin) {
    fin.read((char*)&obj, sizeof(obj));
    if(fin.eof()) break;

    if (obj.animal_index == animal_index)
        found = 1;

    else
       fout.write((char*)&obj, sizeof(obj));
  }
  fout.close();
  fin.close();

   if (found == 1) {
      remove("animal_list.dat");
      result=rename("temp.dat", "animal_list.dat");//rename returns 0 if deletion is successful
      if(result==0)
        cout<<" Deletion successful\n";
  }

  else {
    remove("temp.dat");
    cout << "Sorry animal index was invalid\n";
    }
    fin.close();
    fout.close();
    }
  else
   cout<<"Sorry file is empty\n";
}

void admin_add() {
  admin obj;
  fstream fout("admin_list.dat", ios::in | ios::out | ios::binary);
  fout.seekp(0, ios::end);
  cout << "Enter admin name\n";
  cin.ignore(100, '\n');  // skips up to 100 characters
                          // stopping after it finds and "grabs" \n
  cin.getline(obj.user_name, 20);
  cout << "Enter admin email\n";
  cin.ignore(100, '\n');  // skips up to 100 characters
                          // stopping after it finds and "grabs" \n
  cin.getline(obj.user_email, 20);
  cout << "Enter admin code\n";
  cin >> obj.admin_code;
  fout.write((char*)&obj, sizeof(obj));
  fout.close();
}
void post_admin_login() {
  int op, temp_location_index;
  char ch;
  cout << "Welcome to exclusive admin powers\n";
  do {
    cout << "Enter\n1. To modify any animal's details\n2. To add an animal\n3. To delete an animal\n4. To view current statistics on animal\n5. To add an admin\n";
    cin >> op;
    switch (op) {
      case 1:
        cout << "Welcome to modification\n";
        modify_animal();
        break;
      case 2:
        cout << "Welcome to Addition\n";
	    add_animal();
        break;
      case 3:
        cout << "Welcome to Deletion\n";
        delete_animal();
        break;
      case 4:
        post_viewer_login();
        break;
      case 5:
        cout << "Welcome to admin addition\n";
        admin_add();
        break;
      default:
        cout << "Sorry invalid choice\n";
    }
    cout << "Continue with main menu(Y/N) ?\n";
    cin>>ch;
    cout<<"\n";
  } while (ch == 'Y'||ch=='y');
}

void admin_login() {
  long int compare;
  int found = 0;
  admin obj_read;               // temporary object for holding and comparison
  cout << "Enter admincode\n";  // to give a unique identity to admin, this is
                                // also their password
  cin >> compare;
  ifstream fin("admin_list.dat", ios::binary);
  fin.seekg(0);
  while (fin) {
    fin.read((char*)&obj_read, sizeof(obj_read));
    if(fin.eof()) break;
    if (obj_read.admin_code == compare) {
      found = 1;
      cout<<"Welcome\n";
      post_admin_login();
     }
  }
  fin.close();
  if (found == 0) cout << "Sorry Admin code doesnt exist\n";
}
void viewer_login() {
  long int viewer_code;
  int found1, found2;
  found1=found2=0;
  viewer obj_read;                // temporary object for holding and comparison
  cout << "Enter viewer code\n";  // to give a unique identity to user
  cin >> viewer_code;
  ifstream fin("viewer_list.dat", ios::binary);
  fin.seekg(0);
  while (fin) {
    found1=1;
    fin.read((char*)&obj_read, sizeof(obj_read));
    if(fin.eof()) break;
    if (obj_read.viewer_code == viewer_code) {
      found2=1;
      cout<<"Welcome\n";
      post_viewer_login();
      break;
    }
  }
  fin.close();
  if (found1 == 0) cout<<"Sorry Viewer list is empty\n";
  else if (found2 == 0) cout << "Viewer code doesnt exist\n";
  else cout<<"Viewing over\n";
}

void viewer_signup() {
  long int ctr = 0;
  viewer obj;
  ifstream fin("viewer_list.dat", ios::binary);
  fin.seekg(0);
  while (fin)  // loop to count existing number of records so that we can

  {
    fin.read((char*)&obj, sizeof(obj));
    if(fin.eof()) break;

    ctr++;// generate viwer code
  }
  fin.close();
  obj.viewer_code = ctr;
  cout << "Enter name\n";
  cin.ignore(100, '\n');  // skips up to 100 characters
                          // stopping after it finds and "grabs" \n
  cin.getline(obj.user_name, 25);
  cout << "Enter email address\n";
  cin.ignore(100, '\n');  // skips up to 100 characters
                          // stopping after it finds and "grabs" \n
  cin.getline(obj.user_email, 25);

  fstream fout("viewer_list.dat", ios::in | ios::out | ios::binary);
  fout.seekp(0, ios::end);
  fout.write((char*)&obj, sizeof(obj));
  cout << "Your viewer code is " << ctr << " Please remember this\n";
  fout.close();
  viewer_login();
}

void entry_menu() {
  char ch1, ch2;
  cout << "For viewer press v, For administrator press a\n";
  cin >> ch1;
  switch (ch1) {
    case 'v':
      cout << "To login enter 'L',to sign up enter 'S'\n";
      cin >> ch2;
      switch (ch2) {
        case 'L':
          viewer_login();
          break;
        case 'S':
          viewer_signup();
          break;
        default:
          cout << "choose login or sign up\n";
      }
      break;
    case 'a':
      admin_login();
      break;
    default:
      cout << "Please enter valid char\n";
  }
}
void main() {
  clrscr();
  cout<< "Welcome to dynamic endangered animal population management system \n";
  ifstream fin("admin_list.dat", ios::binary);
  if (fin)
    entry_menu();
  else {
    cout << "You are the first admin in the endangered animal management "
            "system, please add permanent admins in the next step\n";
    admin_add();
   }
  cout<<"Session ended";
  getch();
}
