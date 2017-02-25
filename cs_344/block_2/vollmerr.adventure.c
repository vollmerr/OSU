/******************************************************************************
* FILE: vollmerr.adventure.c
* DESCRIPTION: Runs adventure game based off rooms already created.
* AUTHOR: Ryan Vollmer
* LAST REVISED: 02/11/2017
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_LEN 1024
#define MAX_BUF 128
#define STR_MAX 64
#define ROOM_COUNT 7

pthread_mutex_t lock;

typedef struct Room {
    char name[STR_MAX];
    struct Room** connections;
    char type[STR_MAX];
    int connected;
} Room;

typedef struct Path {
    char name[STR_MAX];
    struct Path* next;
} Path;

//////////////////////////////////////////////////////////////////////
//////////////////////  PATH FUNCTIONS  //////////////////////////////
//////////////////////////////////////////////////////////////////////
/**
 * Initalizes path user has taken
 * First path will be ignored during printing
 * @param {Path*} path    - path to initalize
 */
void init_path(Path** path) {
    *path = malloc(sizeof(Path));             // allocate new path
    if (!path) exit(1);                       // failed to allocated, exit
    (*path)->next = NULL;                     // set as end of path
}

/**
 * Adds a new Path to users path taken
 * @param {Path*} path    - path user has taken
 * @param {char*} name    - name of path to add
 */
void add_path(Path** path, char* name) {
    Path* cur = *path;
    while (cur->next) cur = cur->next;        // find last path
    cur->next = malloc(sizeof(Path));         // allocate new path
    if (!cur->next) exit(1);                  // failed to allocated, exit
    strcpy(cur->next->name, name);            // copy name into new paths name
    cur->next->next = NULL;                   // set new path as last
}

/**
 * Deallocate all paths
 * @param {Path*} path    - head of path list
 */
void free_paths(Path* path) {
    Path* prev;
    while (path->next) {                    // while there is another path
        prev = path;                        // cache the current path
        path = path->next;                  // move to next path
        free(prev);                         // free old current path
    }
    free(path);                             // free remaining path
}

//////////////////////////////////////////////////////////////////////
//////////////////////  ROOM FUNCTIONS  //////////////////////////////
//////////////////////////////////////////////////////////////////////
/**
 * Initalizes file path to use for populating rooms from files
 * Finds most recent rooms directory
 * @param {char*} file_path   - path to initalize
 */
void get_room_path(char* file_path) {
    DIR* dp = NULL;
    struct dirent* cur_dir = NULL;
    struct stat s;
    time_t recent_time = 0;
    // open the current directory
    if (!(dp = opendir("."))) {
        printf("Err: Failed to open dir\n");
        exit(1);
    }
    // for each file in dir
    while((cur_dir = readdir(dp))) {
        stat(cur_dir->d_name, &s);
        // if a dir, matches format, and more recent
        if ((s.st_mode & S_IFMT) == S_IFDIR &&
        strstr(cur_dir->d_name, "vollmerr.rooms") &&
        s.st_mtime > recent_time) {
            // set dir as path
            strncpy(file_path, cur_dir->d_name, MAX_LEN);
            // update as being the most recent
            recent_time = s.st_mtime;
        }
    }
    closedir(dp);
}

/**
 * Set data for single room
 * @param {Room*} room    - room to add data to
 * @param {char*} buffer  - buffer holding data
 * @param {int} cur_con   - index of current connection
 * @return {int} cur_con
 */
int set_room(Room* room, char* buffer, int cur_con) {
    char data[STR_MAX];
    // remove newline char
    buffer[strlen(buffer) - 1] = '\0';
    // get lines data
    strcpy(data, strstr(buffer, ": ") + 2);
    // if room name
    if (strstr(buffer, "ROOM NAME:")) {
        // set room name
        strcpy(room->name, data);
    }
    // if room type
    else if (strstr(buffer, "ROOM TYPE:")) {
        // set room type
        strcpy(room->type, data);
    }
    // if connection
    else {
        // increment current and total connections
        cur_con++;
        room->connected = cur_con;
    }
    return cur_con;
}

/**
 * Initializes connections of rooms
 * @param {Room*} rooms         - rooms to initalize connecitons for
 */
void init_connections(Room* rooms) {
    int i, j, k;
    char file_path[MAX_LEN];
    char path[MAX_LEN];
    char buffer[MAX_BUF];
    char data[STR_MAX];
    FILE* fp = NULL;
    DIR* dp = NULL;
    struct dirent* cur_file = NULL;
    struct stat s;

    // init path with most recent rooms dir
    get_room_path(path);
    // open the room directory
    if (!(dp = opendir(path))) {
        printf("Err: Failed to open dir\n");
        exit(1);
    }
    i = 0; // room number
    // for each file in directory
    while((cur_file = readdir(dp))) {
        // ignore cur and parent dirs
        if (!strcmp(cur_file->d_name, ".") ||
            !strcmp(cur_file->d_name, "..")) {
            continue;
        }
        // add filename on end of path
        snprintf(file_path, MAX_LEN, "%s/%s", path, cur_file->d_name);
        // open room file
        if (!(fp = fopen(file_path, "r"))) {
            printf("Err: Failed to open file %s\n", file_path);
            exit(1);
        }
        // allocate space for connections
        rooms[i].connections = malloc(rooms[i].connected * sizeof(Room *));
        if (!rooms[i].connections) exit(1);
        j = 0; // connection number
        // for each line in file
        while((fgets(buffer, MAX_BUF, fp))) {
            k = 0; // room connected to number
            // remove newline char
            buffer[strlen(buffer) - 1] = '\0';
            // get lines data
            strcpy(data, strstr(buffer, ": ") + 2);
            // if a connection
            if (strstr(buffer, "CONNECTION")) {
                // while not the room conneciton is
                while (strcmp(data, rooms[k].name)) {
                    // increment which room
                    k = (k + 1) % ROOM_COUNT;
                }
                // set room as connection
                rooms[i].connections[j] = &rooms[k];
                j++;
            }
        }
        // close room file
        fclose(fp);
        // increment room number
        i++;
    }
    // closse rooms dir
    closedir(dp);
}

/**
 * Initalize rooms to data from files created by buildrooms
 * @param {Room*} rooms     - rooms to initialize
 * @param {Room} room       - room player will start in
 */
void init_rooms(Room* rooms, Room** room) {
    int i, j;
    char file_path[MAX_LEN];
    char path[MAX_LEN];
    char buffer[MAX_BUF];
    FILE* fp = NULL;
    DIR* dp = NULL;
    struct dirent* cur_file = NULL;
    struct stat s;
    // init path with most recent rooms dir
    get_room_path(path);
    // open the room directory
    if (!(dp = opendir(path))) {
        printf("Err: Failed to open dir\n");
        exit(1);
    }
    i = 0;
    // for each file in directory
    while((cur_file = readdir(dp))) {
        // ignore cur and parent dirs
        if (!strcmp(cur_file->d_name, ".") ||
            !strcmp(cur_file->d_name, "..")) {
            continue;
        }
        // add filename on end of path
        snprintf(file_path, MAX_LEN, "%s/%s", path, cur_file->d_name);
        // open room file
        if (!(fp = fopen(file_path, "r"))) {
            printf("Err: Failed to open file %s\n", file_path);
            exit(1);
        }
        j = 0;
        // for each line in file
        while((fgets(buffer, MAX_BUF, fp))) {
            // set number of connected rooms, room type, and room name
            j = set_room(&rooms[i], buffer, j);
        }
        // set start room as start location
        if (!strcmp(rooms[i].type, "START_ROOM")) {
            *room = &rooms[i];
        }
        // close room file
        fclose(fp);
        // increment room number
        i++;
    }
    // closse rooms dir
    closedir(dp);
}

/**
 * Unallocates rooms connections array
 * @param {Room*} rooms     - rooms to free
 */
void free_rooms(Room* rooms) {
    int i, j;
    for (i=0; i<ROOM_COUNT; i++) {
        free(rooms[i].connections);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////  TIME FUNCTIONS  //////////////////////////////
//////////////////////////////////////////////////////////////////////
/**
 * Subthread that gets current time and sets to file
 */
void* get_time(void* arg) {
    pthread_mutex_lock(&lock);
    FILE* fp = fopen("currentTime.txt", "w");
    time_t rawtime;
    struct tm *info;
    char buffer[80];
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer,80,"%I:%M%p, %A, %B %d, %Y", info);
    fprintf(fp, "%s\n", buffer );
    fclose(fp);
    pthread_mutex_unlock(&lock);
    return NULL;
}

//////////////////////////////////////////////////////////////////////
//////////////////////  GAME FUNCTIONS  //////////////////////////////
//////////////////////////////////////////////////////////////////////
/**
 * Displays the prompt for the user with a list of possible inputs
 * @param {Room*} room      - Current room user is in
 */
void print_prompt(Room* room) {
    int i;
    char punc[STR_MAX];
    printf("CURRENT LOCATION: %s\n", room->name);
    // print possible connections
    printf("POSSIBLE CONNECTIONS: ");
    for (i=0; i<room->connected; i++) {
        strcpy(punc, i == room->connected - 1 ? ".\n" : ", ");
        printf("%s%s", room->connections[i]->name, punc);
    }
}

/**
 * Sets connection as current room if matches user input
 * @param {Room**}  room     - current room
 * @param {Path**}  path     - path user has taken
 * @param {int*}    steps    - number of steps taken
 */
void get_new_room(Room** room, Path** path, int* steps) {
    int i, found;
    char buffer[STR_MAX];
    pthread_t time_thread;
    FILE* fp;
    printf("WHERE TO? >");
    // get users input
    fgets(buffer, STR_MAX, stdin);
    printf("\n");
    // remove newline char
    buffer[strlen(buffer) - 1] = '\0';
    i=0;
    found = 0;
    if (!strcmp(buffer, "time")) {
        pthread_create(&time_thread, NULL, get_time, NULL);
        pthread_join(time_thread, NULL);
        fp = fopen("currentTime.txt", "r");
        fgets(buffer, STR_MAX, (FILE*)fp);
        printf("%s\n", buffer);
        fclose(fp);
        printf("WHERE TO? >");
        // get users input
        fgets(buffer, STR_MAX, stdin);
        printf("\n");
        // remove newline char
        buffer[strlen(buffer) - 1] = '\0';
    }
    // for each connection room and while not found
    while (i<(*room)->connected && !found) {
        // if input matches room name
        if (!strcmp(buffer, (*room)->connections[i]->name)) {
            // set current room as connection room
            *room = (*room)->connections[i];
            found = 1;
            // increment number of steps taken
            *steps = *steps + 1;
            // add to path taken
            add_path(path, (*room)->name);
        }
        i++;
    }
    // if correct connection not enetered
    if (!found) {
    // print unknown room message
        printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
    }
}

/**
 * Main loops for game
 * Runs until user reaches end room
 * @param {Room*}   rooms   - rooms for user to navigate through
 * @param {Room**}  room    - current room
 * @param {Path**}  path    - path user has taken
 * @param {int*}    steps   - number of steps user has taken
 */
void game_loop(Room* rooms, Room** room, Path** path, int* steps) {
    // while not in end room
    while(strcmp((*room)->type, "END_ROOM")) {
        // prompt user
        print_prompt(*room);
        // get new room based off user input
        get_new_room(room, path, steps);
    }
}

/**
 * Prints vicory message for user including
 * nuber of steps and path taken
 * @param {Path*} path      - path user has taken
 * @param {int} steps       - number of steps user has taken
 */
void print_victory(Path* path, int steps) {
    // print victory message
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    // print number of steps and path taken
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS: \n", steps);
    while (path->next) {
        printf("%s\n", path->next->name);
        path = path->next;
    }
}

/**
 * Entry point into main thread
 */
void* main_entry(void* arg) {
    int steps = 0;
    Room rooms[ROOM_COUNT];
    Room* room;
    Path* path;
    // initalize rooms and start location
    init_rooms(rooms, &room);
    // initalize where rooms connect to
    init_connections(rooms);
    // initalize path user will take
    init_path(&path);
    // play game
    game_loop(rooms, &room, &path, &steps);
    // game finished, print number of steps and path
    print_victory(path, steps);
    // free all paths
    free_paths(path);
    // free all rooms
    free_rooms(rooms);
}

int main() {
    pthread_t main_thread;
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        exit(1);
    }
    pthread_create(&main_thread, NULL, main_entry, NULL);
    pthread_join(main_thread, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}
