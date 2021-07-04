#include <stdio.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination){
	// TODO 3.1creeaza un pachet cu prioritate si destinatia date. Destinatia se va
//duplica ı̂n cazul ı̂n care este diferita de NULL, altfel va fi nul
	Package *package = (Package*)malloc(sizeof(Package));
	package->priority = priority;
	package->destination = (char*)malloc(50*sizeof(char));
	if(destination == NULL){
		package->destination = NULL;
	} else {	
		char *nou = strdup(destination);
		package->destination = nou;
		}
	return package;
}

void destroy_package(Package* package){
	// TODO: 3.1
	if(package->destination != NULL){
		free(package->destination);
	}
	free(package);
}

Manifest* create_manifest_node(void){
	// TODO: 3.1
	Manifest* manifest = (Manifest*)malloc(sizeof(Manifest));
	manifest->package = (Package*)malloc(sizeof(Package));
	manifest->package->destination = (char*)malloc(50*sizeof(char));
	manifest->package = NULL;
	manifest->next = NULL;
	manifest->prev = NULL;
	return manifest;
}

void destroy_manifest_node(Manifest* manifest_node){
	// TODO: 3.1.
	Manifest *aux = manifest_node->next;
	Manifest *aux2 = manifest_node->prev;
	if(aux!=NULL){
		manifest_node->package = aux->package;
		manifest_node->next = aux->next;
		free(aux);
	} else if(aux2!=NULL) {
		manifest_node->package = aux2->package;
		manifest_node->prev = aux2->prev;
		free(aux2);
	} else {
		free(manifest_node);
	}
}


Wearhouse* create_wearhouse(long capacity){
	// TODO: 3.2
	Wearhouse *wearhouse = (Wearhouse*)malloc(sizeof(Wearhouse));
	wearhouse->size = 0;
	wearhouse->capacity = capacity;
	if (wearhouse->capacity !=0 ){
		wearhouse->packages=(Package**)malloc(capacity*sizeof(Package*));
		long i;
		for(i = 0;i < wearhouse->capacity; i++){
			wearhouse->packages[i] = (Package*)malloc(sizeof(Package));
			wearhouse->packages[i]->destination = (char*)malloc(50*sizeof(char));
			wearhouse->packages[i] = NULL;
		}
	}else{
		wearhouse->packages = NULL;
		wearhouse = NULL;
		}
	return wearhouse;

}

Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	// TODO: 3.2
	if ((w->size) != 0){
		return 0;
	} else {
		return 1;
	}
}

long wearhouse_is_full(Wearhouse *w){
	// TODO: 3.2
	if(w->capacity==w->size){
		return 1;
	} else {
		return 0;
	}
}

long wearhouse_max_package_priority(Wearhouse *w){
	// TODO: 3.2
	long i;
	long maxim = 0;
	for(i = 0;i <w->size; i++){
		if(w->packages[i]->priority > maxim){
			maxim = w->packages[i]->priority;
		}
	}
	return maxim;
}

long wearhouse_min_package_priority(Wearhouse *w){
	// TODO: 3.2
	long i;
	long minim = w->packages[0]->priority;
	for(i = 1;i <w->size; i++)
	{
		if(w->packages[i]->priority < minim){
			minim = w->packages[i]->priority;
		}
	}
	return minim;
	}


void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){
	// TODO: 3.2
	long i;
	for(i = 0;i < wearhouse->size; i++){
		free(wearhouse->packages[i]->destination);
		free(wearhouse->packages[i]);
		}
	free(wearhouse->packages);
	free(wearhouse);

}


Robot* create_robot(long capacity){
	// TODO: 3.2
	Robot* robot = (Robot*)malloc(sizeof(Robot));
	robot->manifest = NULL;
	robot->size = 0;
	robot->capacity = capacity;
	robot->next = NULL;
	return robot;
}

int robot_is_full(Robot* robot){
	// TODO: 3.2
	if(robot->size == robot->capacity){
		return 1;
	}
	return 0;
}

int robot_is_empty(Robot* robot){
	// TODO: 3.2
	if(robot->size==0){
		return 1;
	}
	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	// TODO: 3.2
	long i;
	Package *nou;
	int ok = 0;
	for(i = 0;i <w->size && ok == 0; i++){
		if(w->packages[i]->priority == priority)
		{
			ok = 1;
			nou = w->packages[i];
		}
	}
	if (ok ==1 )
		return nou;
	else
		return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	// TODO: 3.2
	long i, j;
	int ok = 0;
	for(i = 0;i <w->size && ok == 0; i++){
		if(strcmp(w->packages[i]->destination,package->destination) == 0 && w->packages[i]->priority == package->priority)
		{	
			 j = i;
			ok = 1;
		}
	}
	while((j < w->size-1) &&(w->size-1) != 0 ){
		w->packages[j] = w->packages[j+1];
		j++;
	}
	w->size--;
			
}
	

void robot_load_one_package(Robot* robot, Package* package){
	Manifest *node = create_manifest_node();
	node->package = package;
	Manifest *current;
	Manifest *aux;
	if(robot->manifest == NULL && robot->size<robot->capacity){
		robot->manifest = node;
		robot->size++;
	}
	else if(robot->manifest->package->priority<node->package->priority && robot->size<robot->capacity)
	{
		node->next=robot->manifest;
		node->next->prev = node;
		robot->manifest = node;
		robot->size++;
	}
	else if(robot->manifest->package->priority==node->package->priority && strcmp(robot->manifest->package->destination, node->package->destination) > 0 && robot->size<robot->capacity)
	{	
		node->next=robot->manifest;
		node->next->prev = node;
		robot->manifest = node;
		robot->size++;
	}
	else if(robot->manifest->package->priority==node->package->priority && strcmp(robot->manifest->package->destination, node->package->destination)==0 && robot->size<robot->capacity){
		aux = robot->manifest;
		while(aux->next!=NULL  && aux->next->package->priority== node->package->priority && strcmp(aux->next->package->destination, node->package->destination)==0)
			aux = aux->next;
		node->next=aux->next;
		node->prev=aux;
		aux->next->prev=node;
		aux->next=node;
		robot->size++;
	}

	else if (robot->size<robot->capacity){
		current = robot->manifest;
		while((current->next!=NULL && current->next->package->priority > node->package->priority) || (current->next!=NULL && current->next->package->priority== node->package->priority && strcmp(current->next->package->destination, node->package->destination) <= 0))
			current = current->next;
		if(current->next==NULL)
			node->next=NULL;
		else
			node->next = current->next;
		if (current->next != NULL) 
            node->next->prev = node; 		
		current->next=node;
		node->prev = current;
		robot->size++;
	}	
}

	
long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	long l=0;
	long j;
	Package *aux;
	long maxim = wearhouse_max_package_priority(wearhouse);
	j = maxim;
		while(robot_is_full(robot) == 0){
			j = maxim;
			if(robot_get_wearhouse_priority_package(wearhouse,j) != NULL){
				aux = robot_get_wearhouse_priority_package(wearhouse,j);
				robot_load_one_package(robot,aux);
				robot_remove_wearhouse_package(wearhouse,aux);
				l++;
				maxim = wearhouse_max_package_priority(wearhouse);
			}
				
		}
	
	if(wearhouse->size == 0)
		return 0;
	else
		return l;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	// TODO: 3.2
	//long i;
	Package *aux;
	Manifest *nod=robot->manifest;
	long maxim = 0;
	while(nod->next != NULL){
		if(strcmp(nod->package->destination,destination) == 0){
			if(maxim < nod->package->priority){
				maxim = nod->package->priority;
				aux = nod->package;
			}
		}
		nod = nod->next;
	}
	return aux;
}

void destroy_robot(Robot* robot){
	long i;
	for(i = 0;i < robot->size; i++){
		free(robot->manifest[i].package->destination);
		free(robot->manifest[i].package);
	}
	free(robot->manifest);
	robot->next = NULL;
	free(robot);
}

void robot_unload_packages(Truck* truck, Robot* robot){

	Manifest *aux,*aux1;
	Manifest *parcurgere = robot->manifest;
	Manifest *ant = parcurgere;

	while(parcurgere->next != NULL &&  truck->size < truck->capacity ){
		if(strcmp(truck->destination,parcurgere->package->destination)==0){
			Manifest *nou = create_manifest_node();	
			nou->package = parcurgere->package;
			nou->prev = NULL;
			nou->next = NULL;
			if(parcurgere->prev == NULL && parcurgere->next == NULL){
				if(truck->size==0){
					truck->manifest=nou;
					if(truck->size<truck->capacity){
						truck->size++;
					}
				} else {
					aux = truck->manifest;
					while(aux->next != NULL){
						aux = aux->next;
					}
					aux->next = nou;
					nou->prev = aux;
					if(truck->size < truck->capacity)
						truck->size++;
					}
					robot->size--;
			} else if(parcurgere->prev == NULL){
					if(truck->size == 0){
						truck->manifest=nou;
					if(truck->size < truck->capacity)
						truck->size++;
					} else{
						aux = truck->manifest;
						while(aux->next != NULL){
							aux = aux->next;
						}
						aux->next = nou;
						nou->prev = aux;
						if(truck->size < truck->capacity)
							truck->size++;
					}
				robot->manifest = robot->manifest->next;
				robot->manifest->prev = NULL;
				parcurgere = robot->manifest;
			} else if ( parcurgere->next != NULL && parcurgere->prev != NULL){
					if(truck->size == 0){
						truck->manifest = nou;
						if(truck->size < truck->capacity)
							truck->size++;
					} else{
						aux1 = truck->manifest;
						while(aux1->next != NULL){
							aux1 = aux1->next;
						}
						aux1->next = nou;
						nou->prev = aux1;
						if(truck->size < truck->capacity)
							truck->size++;
						}
				parcurgere = parcurgere->next;
				ant->next = parcurgere;
				parcurgere->prev = ant;
			}
			robot->size--;
		} else {
		 	 ant = parcurgere;
		 	 parcurgere = parcurgere->next;
		}
	}	
	if(truck->size < truck->capacity){
		if(strcmp(parcurgere->package->destination, truck->destination) == 0){
			Manifest *nou = create_manifest_node();	
			nou = parcurgere;
			if(truck->size == 0){
				truck->manifest = nou;
				if(truck->size < truck->capacity)
					truck->size++;
			}else{
				aux = truck->manifest;
				while(aux->next != NULL){
						aux = aux->next;
				}
				aux->next = nou;
				nou->prev = aux;
				if(truck->size < truck->capacity)
					truck->size++;
			}
				robot->size--;
		}
	}
			

}

// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){
	// TODO: 3.3
	Truck* truck = (Truck*)malloc(sizeof(Truck));
	truck->size = 0;
	truck->capacity = capacity;
	truck->manifest = create_manifest_node();
	truck->manifest = NULL;
	truck->unloading_robots = create_robot(capacity);
	truck->unloading_robots = NULL;
	truck->destination = (char*)malloc(50*sizeof(char));
	if(destination != NULL){
		char *nou = strdup(destination);
		truck->destination=nou;
	} else {
		truck->destination=NULL;
	}
	truck->in_transit_time = 0;
	truck->transit_end_time = transit_time;
	truck->departure_time = departure_time;
	truck->next = NULL;
	
	return truck;
}

int truck_is_full(Truck *truck){
	// TODO: 3.3
	if(truck->capacity == truck->size){
		return 1;
	} else {
		return 0;
	}
}

int truck_is_empty(Truck *truck){
	// TODO: 3.3
	if(truck->size == 0){
		return 1;
	} else { 
		return 0;
	}
}

long truck_destination_robots_unloading_size(Truck* truck){
	// TODO: 3.3
	long nrpack = 0;
	while( truck->unloading_robots != NULL){
		if(strcmp(truck->unloading_robots->manifest->package->destination,truck->destination) == 0)	{
			while(truck->unloading_robots->manifest != NULL){
				nrpack++;
				truck->unloading_robots->manifest = truck->unloading_robots->manifest->next;
			}
		}
		truck->unloading_robots = truck->unloading_robots->next;
	}

	return nrpack;
}


void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck){
	// TODO: 3.3
	while(truck->manifest != NULL){
		free(truck->manifest->package->destination);
		free(truck->manifest->package);
		truck->manifest = truck->manifest->next;
	}
	free(truck->manifest);
	while(truck->unloading_robots!=NULL){
		while(truck->unloading_robots->manifest!=NULL){
			free(truck->unloading_robots->manifest->package->destination);
			free(truck->unloading_robots->manifest->package);
			truck->unloading_robots->manifest = truck->unloading_robots->manifest->next;
		}
		free(truck->unloading_robots->manifest);
		truck->unloading_robots = truck->unloading_robots->next;
	}
	free(truck->unloading_robots);
	free(truck->destination);
	
}


Parkinglot* create_parkinglot(void){
	// TODO: 3.4
	// Allocate parking lot
	Parkinglot *parkinglot = (Parkinglot*)malloc(sizeof(Parkinglot));
	parkinglot->arrived_trucks = create_truck(0,0,0,0);
	parkinglot->arrived_trucks->next = parkinglot->arrived_trucks;
	parkinglot->departed_trucks = create_truck(0,0,0,0);
	parkinglot->departed_trucks->next = parkinglot->departed_trucks;
	parkinglot->pending_robots = create_robot(0);
	parkinglot->pending_robots->next = parkinglot->pending_robots;
	parkinglot->standby_robots = create_robot(0);
	parkinglot->standby_robots->next = parkinglot->standby_robots;

	return parkinglot;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	// TODO: 3.4
	long minim = 10000;
	long maxim = 0;
	Robot *current;
	if(robot->size == 0){	
		if(parkinglot->standby_robots == parkinglot->standby_robots->next){
			parkinglot->standby_robots->next = robot;
			robot->next = parkinglot->standby_robots;
		} else if(parkinglot->standby_robots == parkinglot->standby_robots->next->next) {
				current = parkinglot->standby_robots->next;
				if(current->capacity > robot->capacity){
					current->next = robot;
					robot->next = parkinglot->standby_robots;
				} else{
					robot->next = current;
					parkinglot->standby_robots->next = robot;
				}
		}else{
			minim = 10000;
		 	maxim = 0;
			current = parkinglot->standby_robots->next;
			while(current->next != parkinglot->standby_robots){	
				if(current->capacity < minim){
					minim = current->capacity;
				}
				if(current->capacity > maxim)	{
					maxim = current->capacity;
				}
				current = current->next;
			}
			if(minim >= robot->capacity){
				current = parkinglot->standby_robots->next;
				while(current->next != parkinglot->standby_robots){
					current = current->next;
				}
				current->next = robot;
				robot->next = parkinglot->standby_robots;

			}else if(minim < robot->capacity && maxim > robot->capacity){
				  current = parkinglot->standby_robots->next;
				  while(current->next->capacity>robot->capacity){
						current = current->next;
					}
			robot->next = current->next;
			current->next = robot;
		}else if(maxim <= robot->capacity) {
				robot->next = parkinglot->standby_robots->next;
				parkinglot->standby_robots->next = robot;
			}
		}

	} else if(robot->size != 0){
		if(parkinglot->pending_robots == parkinglot->pending_robots->next){ 
			parkinglot->pending_robots->next=robot;
			robot->next=parkinglot->pending_robots;
		}
		else if(parkinglot->pending_robots == parkinglot->pending_robots->next->next){
				current = parkinglot->pending_robots->next;
				if(current->size > robot->size)
				{
					current->next = robot;
					robot->next = parkinglot->pending_robots;
				}
				else
				{
					robot->next = current;
					parkinglot->pending_robots->next = robot;
				}
		}else{
			minim = 10000;
			maxim = 0;
			current = parkinglot->pending_robots->next;
			while(current->next != parkinglot->pending_robots){	
				if(current->size < minim){
					minim = current->size;
				}
				if(current->size > maxim){
					maxim = current->size;
				}
				current = current->next;
			}
			if(minim >= robot->size){
				current = parkinglot->pending_robots->next;
				while(current->next != parkinglot->pending_robots){
					current = current->next;
				}
				current->next = robot;
				robot->next = parkinglot->pending_robots;
			} else if(minim < robot->size && maxim > robot->size){
				current = parkinglot->pending_robots->next;
				while(current->next->size > robot->size){
					current = current->next;
				}
				robot->next = current->next;
				current->next = robot;
				} else if(maxim <= robot->size) {
					current = parkinglot->pending_robots->next;
					robot->next = current;
					parkinglot->standby_robots->next = robot;
				}

		}
	}
	
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){
	// TODO: 3.4
	Robot *p = parkinglot->pending_robots->next;
	Robot *lasts = parkinglot->standby_robots->next;
	Robot *s = parkinglot->standby_robots->next;
	Robot *i = parkinglot->standby_robots->next;
	Robot *j = parkinglot->pending_robots->next;
	Robot *last = parkinglot->pending_robots->next;
	int ok = 0,ik = 0;
	if(s->next == parkinglot->standby_robots){
		if(s != parkinglot->standby_robots && s == robot){
			parkinglot->standby_robots->next = parkinglot->standby_robots;
		}
	} 

	 if(p->next == parkinglot->pending_robots){
		if(p != parkinglot->pending_robots && p == robot){
			parkinglot->pending_robots->next = parkinglot->pending_robots;
		}
	}

	while(i->next != parkinglot->standby_robots && ok == 0){
		if(i == robot){	
			ok = 1;
		}
		lasts = i;
		i = i->next;
	}

	while(j->next!=parkinglot->pending_robots && ik==0){
		if(j == robot){
			ik = 2;
		}
		last = j;
		j = j->next;
	}
	if(lasts == robot && lasts->next != parkinglot->standby_robots){
		parkinglot->standby_robots->next = i;
	}else if(i == robot && i->next == parkinglot->standby_robots){
		lasts->next = parkinglot->standby_robots;
	} else if(ok == 1){
		if(parkinglot->standby_robots->next == i && i == robot && i->next != parkinglot->standby_robots){
				parkinglot->standby_robots->next = i;
				lasts->next = NULL;
		}else if(i == robot && parkinglot->standby_robots->next != i && i->next != parkinglot->standby_robots){
			lasts->next = i->next;
			i->next = NULL;
		}
	  }

	if(last == robot && last->next != parkinglot->pending_robots){
		parkinglot->pending_robots->next = j;
	}else if(j == robot && j->next == parkinglot->pending_robots){
		last->next = parkinglot->pending_robots;
	}else if(ik == 2){
		if(parkinglot->pending_robots->next == j && j == robot && j->next != parkinglot->pending_robots){
				parkinglot->pending_robots->next = j;
				last->next = NULL;
		}else if(j == robot && parkinglot->pending_robots->next != j && j->next != parkinglot->pending_robots){
			last->next = j->next;
			j->next = NULL;
		}
	}

}


int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	// TODO: 3.4
	Robot *current;
	current = parkinglot->pending_robots->next;
	if(current != parkinglot->pending_robots){
		return 1;
	} else{
		return 0;
	}
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	// TODO: 3.4
	Truck *aux;
	aux = parkinglot->arrived_trucks->next;
	int ok = 0;
	if(aux == parkinglot->arrived_trucks){
		ok = 1;
	}else{
		while(aux->next != parkinglot->arrived_trucks){
			if(aux->size != 0){
				ok = 1;
				break;
			}
		aux = aux->next;
		}
	}
	return ok;

}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	// TODO: 3.4
	if(parkinglot->departed_trucks != parkinglot->departed_trucks->next){
		return 1;
	} else{
		return 0;
	}
}


void destroy_parkinglot(Parkinglot* parkinglot){
	// TODO: 3.4
	Truck *aux = parkinglot->arrived_trucks->next;
	Truck *aux1;
	while(aux !=parkinglot->arrived_trucks){
		aux1 = aux;
		aux = aux->next;
		destroy_truck(aux1);
	}
	Truck *aux2 = parkinglot->departed_trucks->next;
	Truck *aux3;
	while(aux2 != parkinglot->departed_trucks){
		aux3 = aux2;
		aux2 = aux2->next;
		destroy_truck(aux3);
	}
	Robot *r = parkinglot->pending_robots->next;
	Robot *help;
	while(r != parkinglot->pending_robots){
		help= r ;
		r = r->next;
		destroy_robot(help);
	}
	Robot *r1 = parkinglot->standby_robots->next;
	Robot *help1;
	while(r1 != parkinglot->standby_robots){
		help1 = r1;
		r1 = r1->next;
		destroy_robot(help1);
	}

	free(parkinglot);

}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}


void truck_departed(Parkinglot *parkinglot, Truck* truck){
	// TODO: 3.5
	// Search through arrived list, if exists node is found remove it
	// Note: this must remove the node from the list, NOT deallocate it
	Truck *aux = parkinglot->departed_trucks->next;
	Truck *prev;
	Truck *ant = parkinglot->arrived_trucks->next;
	Truck *i = parkinglot->arrived_trucks->next;
	int ok=0;
	int ik=0;

	
	if(i->next == parkinglot->arrived_trucks && i != parkinglot->arrived_trucks && i == truck){
			parkinglot->arrived_trucks->next = parkinglot->arrived_trucks;
	} else if(i->next != parkinglot->arrived_trucks ) {
			while(i->next != parkinglot->arrived_trucks && ik == 0){
				if(i == truck){
					ik = 1;
				}
				ant = i;
				i = i->next;
			}
			if(i == truck && i->next == parkinglot->arrived_trucks){
				ant->next = parkinglot->arrived_trucks;
			} else if(ik == 1){
				if(ant == truck && parkinglot->arrived_trucks->next == ant)	{
					parkinglot->arrived_trucks->next = ant->next;
				}else if(i == truck){
					ant->next = i->next;
					i->next = NULL;
				}
			}

	 }


	if(aux==parkinglot->departed_trucks){
		aux->next=truck;
		truck->next=parkinglot->departed_trucks;
	}else if(aux->next==parkinglot->departed_trucks && aux!=parkinglot->departed_trucks && aux->departure_time>truck->departure_time){
			truck->next=aux;
			parkinglot->departed_trucks->next=truck;
	} else if(aux->next==parkinglot->departed_trucks && aux!=parkinglot->departed_trucks && aux->departure_time<=truck->departure_time){
			aux->next=truck;
			truck->next=parkinglot->departed_trucks;
	}else{
		while(aux->next!=parkinglot->departed_trucks && ok==0)	{	
			 if(aux->departure_time>truck->departure_time)
			 	ok=1;
			else{
			prev=aux;
			aux=aux->next;
			}
		}
		if(ok==1){
			if(parkinglot->departed_trucks->next==aux){
				truck->next=aux;
				parkinglot->departed_trucks->next=truck;
			} else {
				truck->next=aux;
				prev->next=truck;
			}
		} else if(aux->next==parkinglot->departed_trucks && aux->departure_time<truck->departure_time) {	
				if(aux->departure_time<truck->departure_time){
					aux->next=truck;
					truck->next=parkinglot->departed_trucks;
				}else{
					truck->next=aux;
					prev->next=truck;
				}
		} 
	}
}



void truck_arrived(Parkinglot *parkinglot, Truck* truck){
	if(parkinglot == NULL || truck == NULL) return;

	// TODO: 3.5
	// Search through departed list, if exists node is found remove it
	// Note: this must remove the node not deallocate it
	Truck *i = parkinglot->departed_trucks->next;
	Truck *a = parkinglot->departed_trucks->next;
	Truck *ant = parkinglot->departed_trucks->next;
	Truck *prev = parkinglot->arrived_trucks->next;
	int ik = 0;
	long k;

	if(a->next == parkinglot->departed_trucks){
		if(a != parkinglot->departed_trucks && a == truck){
			parkinglot->departed_trucks->next=parkinglot->departed_trucks;
		}
	} else {
		while(i->next!=parkinglot->departed_trucks && ik==0){
			if(i == truck){
				ik = 1;
			}
			ant = i;
			i = i->next;
		}

		if(ant == truck && ant->next != parkinglot->departed_trucks && parkinglot->departed_trucks->next == ant){
			parkinglot->departed_trucks->next = parkinglot->departed_trucks->next->next;
		}else if(i == truck && i->next == parkinglot->arrived_trucks){
			ant->next = parkinglot->departed_trucks;
			i->next = NULL;
		}else if(ik == 1){
			if(parkinglot->departed_trucks->next == i && i == truck && i->next != parkinglot->departed_trucks ){
				parkinglot->departed_trucks->next = i;
				ant->next = NULL;
			}else if(i == truck && parkinglot->departed_trucks->next != i && i->next != parkinglot->departed_trucks){
				ant->next = i->next;
				i->next = NULL;
			}
		}

	 }

	Truck *current;
	if(parkinglot->arrived_trucks->next == parkinglot->arrived_trucks){
		parkinglot->arrived_trucks->next = truck;
		truck->next = parkinglot->arrived_trucks;
		for(k = 0;k < truck->size; k++){
			destroy_package(truck->manifest->package);
			truck->manifest = truck->manifest->next;
		}
		truck->manifest = NULL;
		truck->size = 0;
	}else if(strcmp(parkinglot->arrived_trucks->next->destination,truck->destination) > 0){
		truck->next = parkinglot->arrived_trucks->next;
		parkinglot->arrived_trucks->next = truck;
		for(k = 0;k < truck->size; k++){
			destroy_package(truck->manifest->package);
			truck->manifest = truck->manifest->next;
		}
		truck->manifest = NULL;
		truck->size = 0;
	}else if(strcmp(parkinglot->arrived_trucks->next->destination,truck->destination) ==0  && truck->departure_time < parkinglot->arrived_trucks->next->departure_time){	
		   truck->next = parkinglot->arrived_trucks->next;
		   parkinglot->arrived_trucks->next = truck;
		   for(k = 0;k < truck->size;k++){
				destroy_package(truck->manifest->package);
				truck->manifest = truck->manifest->next;
			}
			truck->manifest = NULL;
			truck->size = 0;
	} else {
		current = parkinglot->arrived_trucks->next;
		while((current->next != parkinglot->arrived_trucks) &&( (strcmp(current->destination,truck->destination)<0) || ((strcmp(current->destination,truck->destination)==0 && current->departure_time<=truck->departure_time)))){	prev=current;
				current = current->next;
			}
		if(current->next == parkinglot->arrived_trucks){
				current->next = truck;
				truck->next = parkinglot->arrived_trucks->next;
				for(k = 0;k < truck->size; k++){
					destroy_package(truck->manifest->package);
					truck->manifest = truck->manifest->next;
				}
				truck->manifest = NULL;
				truck->size = 0;
		} else{
				truck->next = current;
				prev->next = truck;
				for(k = 0;k < truck->size;k++){
					destroy_package(truck->manifest->package);
					truck->manifest = truck->manifest->next;
				}
				truck->manifest = NULL;
				truck->size = 0;
			}
	
	}
}	
		

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	// TODO:  3.5
	Robot *t = truck->unloading_robots;
	Robot *aux;
	do
	{	if(t != NULL){
		aux = t;
		t = t->next;
		parkinglot_add_robot(parkinglot,aux);
		}
	}while(t != NULL);
		truck->unloading_robots = NULL;

}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	// TODO: 3.5
	
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	// TODO: 3.5
	Truck *aux;
	Truck *truck = parkinglot->departed_trucks->next;
	while(truck != parkinglot->departed_trucks){
		aux = truck;
		truck = truck->next;
		aux->in_transit_time = aux->in_transit_time+1;
		if(aux->in_transit_time == aux->transit_end_time){
			aux->in_transit_time = 0;
			truck_arrived(parkinglot,aux);
		}
	}
}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}




