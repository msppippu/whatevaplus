//Still need:
//1. draw the shortest path
//2. tell directions
//3. make a nice user interface

#include "library.h"

#define nopath -999
#define locationsize 29145 //given to us


/*BEGIN*******************************Hash Map Struct***************************************/
struct Link
{
	string name, state, code, lat, lon;
	int linenum;
	Link * next;
	Link(){
		name="";
		state="";
		code="";
		lat="";
		lon="";
		int linenum=0;
		next=NULL;

	}
	Link(string a, string s, string c, string la, string lo, int i, Link * n = NULL)
	{ 
		name = a;
		state = s;
		code = c;
		lat=la;
		lon=lo;
		linenum=i;
		next = n;
	}
};
struct LinkedList
{
	Link * head;
	LinkedList() { head = NULL; }

	//searching one Linked List
	Link* search(string n, string s)
	{ 
		Link * p = head;
		while (p != NULL)
		{			
			if (p->state.compare(s)==0&&p->name.compare(n)==0){
				cout <<"Found "<<n<<", "<<s<<endl;
				return p;
			}
			p = p->next;
		}
		return NULL;
	};

	void printList(){
		Link *p = head;
		while (p != NULL){
			cout << p->name << ", " << p->state << endl;
			p = p->next;
		}
	}

	void insert(string n, string s, string c, string la, string lo, int i)
	{
		head = new Link(n, s, c, la, lo, i, head);
	};

	int length()
	{
		int count = 0;
		Link * p = head;
		while (p != NULL)
		{
			count += 1;
			p = p->next;
		}
		return count;
	};
};
string cleanup(string s)
{
	string r = "";
	for (int i = 0; i < (int)s.length(); i += 1)
	{
		char c = s[i];
		if (isalpha(c))
			r += (char)tolower(c);
	}
	return r;
}
int hashString(string s, int size)
{
	int h = 8192;
	const int len = s.length();
	for (int i = 0; i < len; i += 1)
		h = h * 64 + s[i];
	if (h < 0)
		h = -h;
	return (h % size);
}
class hashMap
{
public:
	hashMap(){
		size = 0;
		data = NULL;
		index = NULL;
		count = 0;
		factor = 0;
		defSize = 0;
	};

	hashMap(double fac, int def){
		factor = fac;
		defSize = def;
		size = defSize;
		count = 0;

		data = new LinkedList[size];
		index = data;
	};

	~hashMap(){
		if (data != NULL){
			delete[] data;
		}
	};

	//Return functions for protected values
	int getSize(){ return size; };
	int getCount(){ return count; };
	/*
	LinkedList getValue(const int i){ 
		if (i >= 0 && i < count) return data[i];
		else return LinkedList();
	};
	*/

	//searching the whole map
	Link * searchMap(string n, string s){
		//allow case-insensitive search
		n = cleanup(n);
		s = cleanup(s);
		LinkedList* h = data;
		int i=0;
		Link *tmp=new Link();
		while (i<size) {
			if((tmp=h->search(n,s))!=NULL){
				cout<<" at index "<<i<<endl;
				return tmp;
			}
			h += 1;
			i = i + 1;
		}
		//if we reach the end of the map without finding the value
		if (i==size) {
			cout << "Not found" << endl;
			return NULL;
		}
	};

	void addToMap(string, string, string, string, string, int);
	void addNode(Link, LinkedList*);
	void printMap();

private:
	int size;
	LinkedList *data;
	LinkedList *index;
	int count;
	double factor;
	int defSize;
};
//adding a Link node at index i
void hashMap::addNode(Link l, LinkedList* i){
	//if it is an empty index, insert a new node as the head of a linked list
	if (i == NULL){
		*(i->head) = l;
	}
	//if it is an occupied index, insert a new node in the existing linked list
	else{
		i->insert(l.name, l.state, l.code, l.lat, l.lon, l.linenum);
	}
}
void hashMap::addToMap(string a, string s, string co, string la, string lo, int i){
	a = cleanup(a);
	s = cleanup(s);
	int c = hashString(a, size);
	Link *n = NULL;
	Link l(a, s, co, la, lo, i, n);

	index = data + c;

	if (data != NULL){
		//increment if there are double the size amount of elements
		if (count > (size*2)){
			//cout << "size increased to: " << (int)(size*factor) << "\n";
			//creating new a linked list with bigger size
			LinkedList *newData = new LinkedList[(int)(size*factor)];
			//reassigning old data to new map with new index
			for (int n=0; n<size; n++) {
				//goes through each index and reassigns each node of each linked list to a new hash table
				Link * p = (data + n)->head; //start with head
				//finish one linked list
				while (p != NULL){
					addNode(*p, newData + hashString(p->name, (int)size*factor));
					p = p ->next;
				}
			}
			//delete the old data
			delete[] data;
			//reassign the values
			size = (int)size*factor;
			data = newData;
			int newc = hashString(s, size);
			index = data + newc;
		}
			addNode(l, index);
			count += 1;
	}
}
void hashMap::printMap(){
	LinkedList *h = data;
	int i = 0;
	while (h < (data+size)){
		cout << "At index " << i<< ":\n";
		h->printList();
		h = h + 1;
		i = i + 1;
	}
}
/*END**********************************Hash Map Struct***************************************/




/*BEGIN**********************************Locations structs***********************************/
struct road;
struct location
{
	double lat;
	double lon;

	double disttonear;	
	
	string typeofnear;
	string stateofnear;
	string nameofnear;

	location(double,double,double, string,string,string);
};
location::location(double longi, double latit, double distto, string type, string state, string name)
{
	lon = longi;
	lat = latit;
	disttonear = distto;
	typeofnear = type;
	stateofnear = state;
	nameofnear = name;
}
struct road
{
	string name;
	string desig;
	int loc1;
	int loc2;

	double len;

	location * whereto;
	string direction;
	road(){
		whereto=NULL;
	}
	road(string,string,int,int,double);
};
road::road(string n,string d,int l1,int l2,double length)
{
	name = n;
	desig = d;
	loc1 = l1;
	loc2 = l2;
	len = length;
	whereto = NULL;
	direction="";
}
struct closest
{
	int loc;
	double distance;
	closest(int l,double d){
		loc=l;
		distance=d;
	};
};
/*END************************************Locations structs***********************************/




/*BEGIN********************************Helper Functions**************************************/
void setDirection(road *r, location *l)
{	
	double x1,x2,y1,y2, dir;
	y1=l->lat;
	x1=l->lon;
	y2=r->whereto->lat;
	x2=r->whereto->lon;

	dir = atan2((y2-y1),(-x1)-(-x2))*180.0/(3.14159);
		
	if((-45<=dir && dir<=45)||(dir>=315 && dir<=-315)) r->direction = "East";
	else if((45<dir && dir<=135)||(dir<-225 && dir>-315)) r->direction = "North";
	else if((135<dir && dir<=225)||(dir<-135 && dir>=-225)) r->direction = "West";				
	else r->direction = "South";	
}
/*END**********************************Helper Functions**************************************/




/*BEGIN**************************Draw Map from Coverage.txt*********************************/
string getTile(int lat1, int lat2, int lon1, int lon2, int * lat_ran1, int * lat_ran2, int * lon_ran1, int * lon_ran2){
	ifstream fin;
	fin.open("coverage.txt");
	int latt1, long1, latt2, long2;
	string dat, datTile;
	int latdif=-1, longdif;
	if (fin.fail()) {cout << "Cannot open coverage.txt" << endl;
		return "0";
	}
	else if (fin.is_open()){		
		while(!fin.eof()){
			fin >> latt1 >> latt2>> long1 >> long2 >> dat;

			if (latt1 >= lat1 && latt2 <= lat2 && long1 <= lon1 && long2 >= lon2 ){ //if it is within the range
				if (latdif==-1){ //if first time found within the range
					//store the differences and make note of the dat file
					latdif = latt1 - latt2;
					longdif = long2 - long1;
					*lat_ran1 = latt1;
					*lat_ran2 = latt2;
					*lon_ran1 = long1;
					*lon_ran2 = long2;
					datTile = dat;
				}
				else { //if another tile was found within the range and it is smaller than the one already found, update
					if (latdif > (latt1 - latt2) || longdif > (long2-long1)){
						latdif = latt1-latt2;
						longdif = long2 - long1;
						*lat_ran1 = latt1;
						*lat_ran2 = latt2;
						*lon_ran1 = long1;
						*lon_ran2 = long2;
						datTile = dat;
					}
				}
			}
		}
		fin.close();
		//if we get out of the look without finding a tile within the range, error
		if (latdif == -1) {//if latdif has not been updated to be a non-negative value, I know that we couldn't find one within the range
			cout << "Error: Your locations are out of range!" <<endl;
			return "0";
		}
		else return datTile;
	}
	else return "0";
} 
//function that converts the altitude to a respective color
//takes in minimum and maximum altitudes as well as the special value, spv, to represent water
int colorConvert(int min, int max, int spv, int alt){
	if (alt == spv) {//if water
		return make_color_int(0, 0, 204);
	}
	else {
	double range = (double)(max-min);
	alt = (double)(alt - min);
	return make_color_int(100, (alt / (range/255.0)), 0); //gives a little bit of red and represent the altitude by giving it lighter green as altitude rises
	}
}
//function tha reads and draws the tile
void drawTile(string datTile, int *row, int *column){
	ifstream fin;
	fin.open(datTile.c_str(), ios::in | ios::binary);
	if (fin.fail()) cout << "Cannot open .dat file" <<endl;
	else if(fin.is_open()){
		string dummy;
		int bpp, spp, lls, tls, min, max, spv;
		fin >> dummy >> *row >> dummy >> *column >> dummy 
			>> bpp >> dummy >> spp >> dummy >> lls >> dummy
			>> tls >> dummy >> min >> dummy >> max >> dummy
			>> spv; //the first dummy row
		make_window(*column, *row);
		int c, r;
		fin.seekg(2*(*column), ios::beg);//make sure your cursor is at the right spot
		if(!fin.eof()){
			short int alt;
			int col;
			for (r = 0; r < *row; r+=1){ 
				for (c = 0; c < *column; c+=1){
					fin.read((char *) & alt, bpp);//properly reading the binary file
					col = colorConvert(min, max, spv, alt);//getting the right color value
					set_pixel_color(c, r, col);//fill in the pixel
				}
			}
		fin.close();
		}
	}
}
//draws the point "s" at the x and y coordinates
void drawPoint(int x, int y, int winx, int winy, string s){
	move_to(x, y);
	set_font_angle_degrees(90);
	set_font_size(30);
	set_pen_color(color::white);
	write_string(s, direction::center);
}

void drawMap(double lat1, double lat2, double lon1, double lon2){

	string datTile;
	int lat_ran1, lat_ran2, lon_ran1, lon_ran2, winx, winy;
	datTile = getTile(max(lat1, lat2), min(lat1, lat2), min(lon1,lon2), max(lon1, lon2),&lat_ran1, &lat_ran2, &lon_ran1, &lon_ran2);
	
	if (datTile == "0") cout << "Try again" << endl;
	else {
		cout << "Opening the file: "<<datTile << endl;
		drawTile(datTile, &winy, &winx);
		//giving the right x and y coordinates given longitude/latitude
		int x1=(lon1-lon_ran1)*(double)winx/(double)(lon_ran2-lon_ran1);
		int y1=(lat_ran1-lat1)*(double)winy/(double)(lat_ran1-lat_ran2);
		int x2=(lon2-lon_ran1)*(double)winx/(double)(lon_ran2-lon_ran1);
		int y2=(lat_ran1-lat2)*(double)winy/(double)(lat_ran1-lat_ran2);

		drawPoint(x1, y1, winx, winy, "<1>");
		drawPoint(x2, y2, winx, winy, "<2>");
	}

}
/*END*****************************Draw Map from Coverage.txt*********************************/




/*BEGIN***********************Get Locations from Alphaplaces.txt*****************************/
void loadAlpha(hashMap &h){
	ifstream fin;
	fin.open("alphaplaces.txt");
	if(fin.is_open()){
		string s;
		string ss, ns, cs, las, los;
		int i=0, j;
		while(true){
			if(fin.eof()) break;
			i++;
			getline(fin, s);
			ss=s.substr(0, 2);
			cs=s.substr(2, 7);
			ns=s.substr(9, 35);
			for(j=0;(!(ns[j]==' '&&ns[j+1]==' '))&&j<34; j++);
			ns.resize(j);
			las=s.substr(144, 9);
			if (s[153]==' ')los=s.substr(154,10);
			else los=s.substr(153, 11);
			h.addToMap(ns,ss,cs,las,los,i);
		}
	fin.close();
	}
}
/*END*************************Get Locations from Alphaplaces.txt*****************************/




/*BEGIN*********************************Load Closest.txt*************************************/
void loadClosest(closest c[]){
	ifstream fin("closest.txt");
	if(fin.fail()){
		cout<<"Error opening closest.txt"<<endl;
		return;
	}
	int loc;
	double dis;
	int i=0;
	while(1){
		if(fin.eof()) break;
		fin >> loc >> dis;
		c[i] = closest(loc, dis);
		i++;
	}
	fin.close();
}
/*END***********************************Load Closest.txt*************************************/



/*BEGIN**********************************Priority Queue**************************************/
struct qnode{
	location * data;
	double priority; //cumulative distance
	vector<road*> roads_taken;
	road **roads;
	int numofroads;

	int index;

	qnode(){
		data = NULL;
		priority = nopath;
		roads = NULL;
		numofroads = 0;
		index=-1;
	}

	qnode(location *d, double p){
		data = d;	
		priority = p;
		roads = new road * [15];
		numofroads = 0;
		index = -1;
	}

	~qnode(){
		delete data;
		delete [] roads;
	}
};
class pqueue
{
protected:

	qnode ** qnodes;
	int size;
	int num_item;			//num of items in the heap 
public:
	pqueue();
	void enqueue(location*,double);
	bool isempty();
	qnode * dequeue();
	void adjustprio(qnode*,double, road*,qnode*);
};
pqueue::pqueue()
{
	size = locationsize;		//size of the heap is the total number of locations. We could make the heap a vector but no need to
	num_item = 0;		
	qnodes = new qnode * [locationsize];
	for(int i = 0; i < locationsize; i++)
		qnodes [i] = NULL;
}
bool pqueue::isempty()
{
	return num_item==0;	
}
void pqueue::enqueue(location* t,double p)
{
	int pos=num_item;	

	qnode * tmp = new qnode(t,p);
	qnodes[num_item]=tmp;
	tmp->index=num_item;
	
	num_item++;	
	int parent, left, right;

	//if not empty
	if(num_item>1)	
	{
		parent = (pos-1)/2;
		left = pos*2+1;
		right = pos*2+2;

		//find the right position
		while((qnodes[parent]->priority > p) && pos!=0)
		{  
			//swap
			qnode * temp = qnodes[pos];
			qnodes[pos]=qnodes[parent];
			qnodes[parent]=temp;
			
			//adjust index
			parent=(parent-1)/2;
			pos=(pos-1)/2;
		}
	}
}
//dequeue highest priority item
qnode * pqueue::dequeue()
{
	if(isempty())
	{
		cout <<"Queue is empty\n";
		return NULL;
	}

	qnode *removed = qnodes[0];

	if(num_item>1)
	{
		//swap
		qnode * temp = qnodes[0];
		qnodes[0]=qnodes[num_item-1];
		qnodes[num_item-1]=temp;

		qnodes[num_item-1]->index = -1;
		qnodes[num_item-1]->data = NULL;
		qnodes[num_item-1]->priority = -1;
		qnodes[num_item-1]=NULL;

	}
	num_item--;

	int pos = 0;
	int leftpos = pos*2+1;
	int rightpos = pos*2+2;
	if(qnodes[pos]!=NULL)
	{	
		//check all children
		while(qnodes[leftpos]!=NULL && qnodes[rightpos]!=NULL)
		{
			//if the parent has higher priority, no need to swap
			if(qnodes[pos]->priority < qnodes[leftpos]->priority && qnodes[pos]->priority < qnodes[rightpos]->priority)
				break;
			//Left is smaller than or equal to Right
			if(qnodes[leftpos]->priority <= qnodes[rightpos]->priority) 
			{
					//Swap
					qnode* temp = qnodes[leftpos];
					qnodes[leftpos]=qnodes[pos];
					qnodes[pos]=temp;
					pos=leftpos;
			}
			else //Right is smaller than Left
			{
				//SwapNodes(qnodes[rightpos],qnodes[pos]);
				qnode* temp = qnodes[rightpos];
				qnodes[rightpos]=qnodes[pos];
				qnodes[pos]=temp;
				pos=rightpos;
			}
			leftpos = pos*2+1;	
			rightpos = pos*2+2;
		}
	}
	return removed;
}
void pqueue::adjustprio(qnode *t, double p, road *roadtraveled, qnode *fromwhere)
{
	int i;
	for(i = 0;i < num_item;i++)
	{
		//get the index position
		if(qnodes[i]->data->nameofnear==t->data->nameofnear && 
			qnodes[i]->data->stateofnear==t->data->stateofnear &&
			qnodes[i]->data->lon==t->data->lon && 
			qnodes[i]->data->lat==t->data->lat ) 
		{
			break;
		}
	}
	//if not found
	if(i==num_item)
	{
		return;
	}
	//update priority
	t->priority = p;
	int parent = (i-1)/2;
	int leftpos = i*2+1;
	int rightpos = i*2+2;

	//swap until we have right priority queue
	while(qnodes[i]->priority < qnodes[parent]->priority && i!=0)
	{
		qnode * temp = qnodes[i];
		qnodes[i]=qnodes[parent];
		qnodes[parent]=temp;
		i=(i-1)/2;
		parent=(parent-1)/2;
	}
	//adjusting index...
	while(qnodes[leftpos]!=NULL && qnodes[rightpos]!=NULL)
	{
		if(p < qnodes[leftpos]->priority && p < qnodes[rightpos]->priority)
			break;

		if(qnodes[leftpos]->priority <= qnodes[rightpos]->priority) 
		{
				qnode * temp = qnodes[leftpos];
				qnodes[leftpos]=qnodes[i];
				qnodes[i]=temp;
				i=leftpos;	
		}
		else 
		{
			qnode * temp = qnodes[rightpos];
			qnodes[rightpos]=qnodes[i];
			qnodes[i]=temp;			
			i=rightpos;
		}
	leftpos = i*2+1;	
	rightpos = i*2+2;
	}
	
	t->roads_taken.clear();

	for(i=0;i<fromwhere->roads_taken.size();i++) t->roads_taken.push_back(fromwhere->roads_taken[i]);
	
	t->roads_taken.push_back(roadtraveled);
}
/*END************************************Priority Queue**************************************/



/*BEGIN********************************Load Majorroads.txt***********************************/
void loadMajorroads(qnode *graph[])
{
	ifstream fin("majorroads.txt");
	if(fin.fail())
	{
		std::cout <<"Error opening majorroads.txt!\n";
		exit(1);
	}
	else
	{	string n;
		string d;
		int l1;
		int l2;
		double len;

		while(true)
		{
			if(fin.eof()) break;
			fin>> n >> d >> l1 >> l2 >> len;
			//in miles
			len=len*69;
			road * rd1 = new road(n,d,l1,l2,len);		
			road * rd2 = new road(n,d,l1,l2,len); 

			graph[l1-1]->roads[graph[l1-1]->numofroads] = rd1;	
			graph[l2-1]->roads[graph[l2-1]->numofroads] = rd2;	

			//set whereto location
			graph[l1-1]->roads[graph[l1-1]->numofroads]->whereto = graph[l2-1]->data;
			graph[l2-1]->roads[graph[l2-1]->numofroads]->whereto = graph[l1-1]->data;

			//set the direction
			if(l1==l2){	//if connects to itself			
				rd1->direction = "No Direction";
				rd2->direction = "No Direction";
			}
			else
			{
				setDirection(rd2,graph[l2-1]->data);
				setDirection(rd1,graph[l1-1]->data); 
			}

			//increace the counter for roads connected to a location
			graph[l1-1]->numofroads+=1;
			graph[l2-1]->numofroads+=1;

		}
	}
	fin.close();
	printf("Locations Connected\n\n");
}
/*END**********************************Load Majorroads.txt***********************************/

/*BEGIN********************************Load locations.txt************************************/
void loadLocations(qnode * graph[])
{
	ifstream fin("locations.txt");
	if(fin.fail())
	{
		cout <<"Error opening location.txt!\n";
		return;
	}
	else
	{	string lon, lat, mil, pop, type, state, name;			//double
		int i = 0;
		string s;

		getline(fin,s);//skip the first line

		while(true)
		{
			if(fin.eof()) break;
			fin>>std::skipws>>lon>>lat>>mil>>pop>>type>>state;
			getline(fin,name);									//read an entire line from file
		//	cout<<lon<<" "<<lat<<" "<<mil<<" "<<pop<<" "<<type<<" "<<state<<" "<<name<<endl;
			location * ld = new location(atof(lon.c_str()), atof(lat.c_str()), atof(mil.c_str()), type, state, name);
			graph[i]=new qnode(ld, nopath);
			i++;
		}
	}
	fin.close();
	loadMajorroads(graph);				//fill the array of roads leading from/to each location
}

/*END**********************************Load locations.txt************************************/

/*BEGIN********************************Shortest Path Functions******************************/
double check(qnode *to, pqueue &hp, double d, vector<road*> paths, road * newroad, qnode *from)
{	//already checked or not worth checking
	if(to->priority!=nopath && to->priority <= d) return -1;	
	//if never visited, check
	else if(to->priority==nopath)								
	{
		for(int i=0;i<from->roads_taken.size();i++)
		{
			to->roads_taken.push_back(from->roads_taken[i]);
		}
		to->roads_taken.push_back(newroad);		
		hp.enqueue(to->data,d);
	}
	else if(d < to->priority) hp.adjustprio(to,d,newroad,from);
}

double search(pqueue &hp, qnode *s, qnode *e)
{
	if(s==e) 
	{
		return 0;
	}
	qnode * hn= new qnode;
	qnode * curr= new qnode;
	hp.enqueue(s->data,s->priority);

	int i;
	while(1)
	{	if (hp.isempty()) break;
		i=0;
		hn = hp.dequeue();
		curr = hn;
		double distnow = hn->priority;
		vector<road*> paths;
		for(int k=0;k<curr->roads_taken.size();k++)
		{
			paths.push_back(curr->roads_taken[k]);
		}

		//destination reached!
		if(curr->data->nameofnear==e->data->nameofnear && curr->data->lat==e->data->lat && curr->data->lon==e->data->lon) 
			return distnow;
		
		qnode * next_loc= new qnode;
		double path_len;
		while(i<curr->numofroads)		
		{
			next_loc->data = curr->roads[i]->whereto;
			path_len = curr->roads[i]->len;
			check(next_loc,hp,distnow + path_len,paths, curr->roads[i], curr);
			i++;
		}
	}
}
/*End**********************************Shortest Path Functions*************************************/
void main(){
	
	hashMap hm(2, 50);
	loadAlpha(hm);
	
	string n1, n2, s1, s2;
	
	cout<<"First city name: ";
	cin>>n1;
	cout<<"Which state? ";
	cin>>s1;
	cout<<"Second city name: ";
	cin>>n2;
	cout<<"Which state? ";
	cin>>s2;

	double lat1, lat2, lon1, lon2;

	Link *tmp1=(hm.searchMap("Miami", "FL"));
	lat1=atof(tmp1->lat.c_str());
	lon1=atof(tmp1->lon.c_str());
	
	Link *tmp2=(hm.searchMap("Orlando", "FL"));
	lat2=atof(tmp2->lat.c_str());
	lon2=atof(tmp2->lon.c_str());

	drawMap(lat1, lat2, lon1, lon2);

	
}
