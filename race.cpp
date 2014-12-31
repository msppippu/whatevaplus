#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <sys/resource.h>

using namespace std;

double get_cpu_time()
{ struct rusage ruse;
  getrusage(RUSAGE_SELF, &ruse);
  return ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec/1000000.0 + 
         ruse.ru_stime.tv_sec+ruse.ru_stime.tv_usec/1000000.0; }

struct person{
private:
	string dob, ssn, fn, ln, st;
	int zip;
public:
	person(){
		dob="";
		ssn="";
		fn="";
		ln="";
		st="";
		zip=0;
	};
	person(string d, string ss, string f, string l, string s, int z){
		dob=d;
		ssn=ss;
		fn=f;
		ln=l;
		st=s;
		zip=z;
	};
	void print(){
		cout<<dob<<" "<<ssn<<" "<<fn<<" "<<ln<<" "<<st<<" "<<zip<<endl;
	};
	string getFname() {return fn;};
	string getLname(){return ln;};
	string getDOB(){return dob;};

};

int comparePerson(person a, person b){
  if (a.getLname()>b.getLname()) return 1;
  else if(a.getLname()<b.getLname()) return -1;
  else {
    if(a.getFname()>b.getFname()) return 1;
    else if(a.getFname()<b.getFname()) return -1;
    else {
      if(a.getDOB()>b.getDOB()) return 1;
      else if (a.getDOB()<b.getDOB()) return -1;
      else return 0;
    }
  }
}

void quickSort(person arr[], int left, int right){

  if (left < right) {//if there are less than two items do nothing
    //choose a random pivot index
    srand(time(0));
    int pivot=(rand()%(right - left + 1)) + left;
    //switch the pivot with the right
    person tmp=arr[pivot];
    arr[pivot]=arr[right];
    arr[right]=tmp;

    int i=left - 1; //we'll use do-while
    int j=right; //first item we'll look is (right-1) since the pivot moved to the right
    do{
      do {i++;} while (comparePerson(arr[i], arr[right])<0);
      do {j--;} while (comparePerson(arr[j], arr[right])>0 && (j>left));
      if (i<j){
        //swap
        tmp=arr[i];
        arr[i]=arr[j];
        arr[j]=tmp;
      }
    } while (i<j);
    //put the pivot back to the right spot
    tmp=arr[right];
    arr[right]=arr[i];
    arr[i]=tmp;
    //recursion
    quickSort(arr, left, i-1);
    quickSort(arr, i+1, right);
  }
}

void merge(person arr[], int low, int middle, int high){
  person *tmp = new person [high-low+1];
  int i=low;
  int j=middle+1;
  int k=0;
  while((i<=middle) && (j<=high)){
    if (comparePerson(arr[i], arr[j])<0)
      tmp[k++]=arr[i++];
    else
      tmp[k++]=arr[j++];
  }
  
  while(i<=middle)
    tmp[k++]=arr[i++];
  while (j<=high)
    tmp[k++]=arr[j++];

  for(k=low; k<=high; ++k)
    arr[k]=tmp[k-low];
  delete [] tmp;
}

void mergeSort(person arr[], int low, int high){
  if (low < high) {
    int middle=(low + high)/2;
    mergeSort(arr, low, middle);
    mergeSort(arr, middle+1, high);
    merge(arr, low, middle, high);
  }
}

void main(int argc, char* argv[]){
	ifstream fin;
	if (argc != 4) {
	  cout<<"Try again with correct arguments"<<endl;
	}
	else{
	  fin.open(argv[3], ios::in);
          if(!fin.is_open()){
	    cout<<"Error opening file"<<endl;
	  }
	  else {
	    person p;
	    string d, sn, first, l, s;
	    int z;
	    int size=atoi(argv[2]);
	    person arr[size];
	    for(int i=0; i<size; i++){
		if(!fin.eof()){
		  fin>>d>>sn>>first>>l>>s>>z;
		  p=person(d,sn,first,l,s,z);
		  arr[i]=p;
		}
		else {//if user defined size is larger than the number of items in the file
		  size=i-1;
		  cout<<"Size adjusted to: "<<size<<endl;	
		  person temp[size];
		  std::copy(arr, arr + (size-1), temp);
		  person arr[size];
		  std::copy(temp, temp + (size-1), arr);
		  break;
		}
       	    }
	    double cputime=get_cpu_time();
	    if (strcmp(argv[1],"Q")==0) quickSort(arr, 0, size-1);
	    else if (strcmp(argv[1],"M")==0) mergeSort(arr, 0, size-1);
	    else cout<<"Error: Enter Q or M"<<endl;
	    cputime=get_cpu_time()-cputime;	    
	    for (int i=0; i<size; i++){arr[i].print();}
	    cout<<"The command \""<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<"\" took "<<cputime<<" seconds"<<endl;
          }
        }
}
