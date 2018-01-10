#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ROOM_TOTAL 10
#define ROOM_COUNT 7
#define CONNECTIONS_MIN 3
#define CONNECTIONS_MAX 6

/**
 * Data structure for rooms
 * Name and types reflect indexes in const arrays defined below
 */
typedef struct Room {
  int name;
  int type;
  int connections[CONNECTIONS_MAX];
  int connected;
} Room;

const char* ROOM_NAMES[ROOM_TOTAL] = {"room 1", "room 2", "room 3", "room 4", "room 5", "room 6", "room 7", "room 8", "room 9", "room 10"};
const char* ROOM_TYPES[3] = {"START_ROOM", "END_ROOM", "MID_ROOM"};

/**
 * Initalizes rooms to contain a name, type,
 *   and zero out number of connections.
 *   Selects random indexes in range 0..ROOM_COUNT
 *   to represent which rooms are selected.
 * @param {Room*} rooms          - array of rooms
 * @param {int*} rooms_selected  - array of valid room numbers
 */
void initRooms(Room* rooms, int* rooms_selected) {
  int i, rnd;
  int rooms_selected_total[ROOM_TOTAL] = {0};
  // for all rooms
  for (i=0; i<ROOM_COUNT; i++) {
    // get random number not in selected
    do {
      rnd = rand() % ROOM_TOTAL;
    } while (rooms_selected_total[rnd]);
    // flag room as chosen
    rooms_selected_total[rnd] = 1;
    // add to selected rooms
    rooms_selected[i] = rnd;
    // add name to Room
    rooms[i].name = rnd;
    // add type to Room
    rooms[i].type = !i ? 0 : i > 1 ? 2 : 1;
    // init number of connections to other rooms
    rooms[i].connected = 0;
  }
}

/**
 * Returns valid conneciton number. Connection must not
 *    be self, must not have too many connections already,
 *    and must not already be in connections.
 * @param {Room*} rooms          - array of rooms
 * @param {int*} rooms_selected  - array of valid room numbers
 * @param {int} self             - number of selected room
 * @return {int} con             - number of connection
 */
int getConnectionNumber(Room* rooms, int* rooms_selected, int self) {
  int con, duplicate, i;
  do {
    // get random room index
    con = rand() % ROOM_COUNT;
    // check if connection already in connections
    duplicate = 0;
    i = 0;
    while (i<rooms[self].connected && !duplicate) {
      // already in connecions, set flag
      if (rooms[self].connections[i] == rooms[con].name) {
        duplicate = 1;
      }
      i++;
    }
  } while (duplicate || rooms[self].name == rooms[con].name || rooms[con].connected >= CONNECTIONS_MAX);
  return con;
}

/**
 * Makes connection between two rooms.
 * @param {Room*} rooms         - array of rooms
 * @param {int} self            - current room
 * @param {int} other           - room to connect to
 * @param {int} con_num         - index of connection on self
 */
void setConnections(Room* rooms, int self, int other) {
  rooms[self].connections[rooms[self].connected] = rooms[other].name;
  rooms[self].connected++;
  rooms[other].connections[rooms[other].connected] = rooms[self].name;
  rooms[other].connected++;
}

/**
 * Adds random number of connections between
 *   rooms in range of CONNECTION_MIN and
 *   CONNECTION_MAX to each room
 * @param {Room*} rooms          - array of rooms
 * @param {int*} rooms_selected  - array of valid room numbers
 */
void addConnections(Room* rooms, int* rooms_selected) {
 int i, j, num_cons, con;
  // for all rooms
  for (i=0; i<ROOM_COUNT; i++) {
    // set random number of connections - already connected
    num_cons = rand() % (CONNECTIONS_MAX + 1 - CONNECTIONS_MIN) + CONNECTIONS_MIN - rooms[i].connected;
    if (num_cons > 0) {
      // for all connections in room
      for (j=0; j<num_cons; j++) {
        // find legal random connection
        con = getConnectionNumber(rooms, rooms_selected, i);
        // make connection between room and connected room
        setConnections(rooms, i, con);
      }
    }
  }
}

/**
 * Adds data to file from Room
 * @param {char*} path    - path of file to add room
 * @param {Room} room     - room to add to file
 * @param {int} index     - index of which room to add
 */
void addRoomFile(const char* path, Room room, int index) {
  FILE* fp = NULL;
  int i;
  fp = fopen(path, "w");
  if (fp) {
    fprintf(fp, "ROOM NAME: %s\n", ROOM_NAMES[room.name]);
    for (i=0; i<room.connected; i++) {
      fprintf(fp, "CONNECTION %d: %s\n", i + 1, ROOM_NAMES[room.connections[i]]);
    }
    fprintf(fp, "ROOM TYPE: %s\n", ROOM_TYPES[room.type]);
    fclose(fp);
  }
}

/**
 * Creates folder/file structure based off pid
 * Adds Room structure to associated file
 */
void createFiles(Room* rooms) {
  int i;
  char buffer[64];
  char path[128] = "./vollmerr.rooms.";
  char dir[128];
  snprintf(buffer, sizeof(buffer), "%d", getpid());
  // append pid on end of path
  strcat(path, buffer);
  // copy to reuse later
  strcpy(dir, path);
  // make dir and set permissions
  mkdir(dir, 0777);
  chmod(dir, S_IRWXU | S_IRWXG | S_IRWXO );
  // make file for each room in dir
  for (i=0; i<ROOM_COUNT; i++) {
    // add unique room file to dir
    strcpy(path, dir);
    snprintf(buffer, sizeof(buffer), "/room_%d", i);
    strcat(path, buffer);
    addRoomFile(path, rooms[i], i);
  }
}

int main() {
  int rooms_selected[ROOM_COUNT];
  Room rooms[ROOM_COUNT];
  srand((unsigned)time(NULL)); // seed rand with time

  initRooms(rooms, rooms_selected);
  addConnections(rooms, rooms_selected);
  createFiles(rooms);
  return 0;
}
