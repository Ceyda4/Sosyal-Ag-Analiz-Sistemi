#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 50
#define MAX_USERS 1000
#define MAX_CONNECTIONS 50
#define RED 0
#define BLACK 1

// User structure
typedef struct User {
    int id;
    char name[MAX_NAME_LENGTH];
    int connections[MAX_CONNECTIONS];
    int connection_count;
    float influence_score;
    int community_id;
} User;

// Red-Black Tree Node
typedef struct RBNode {
    int user_id;
    int color;
    struct RBNode *left, *right, *parent;
} RBNode;

// Graph structure
typedef struct Graph {
    User users[MAX_USERS];
    int user_count;
    RBNode *rb_root;
} Graph;

// Visited tracking structures
typedef struct {
    bool visited[MAX_USERS];
} VisitedTracker;

// Stack structure for DFS
typedef struct {
    int items[MAX_USERS];
    int top;
} Stack;

// Queue structure for BFS
typedef struct {
    int items[MAX_USERS];
    int front, rear;
} Queue;

// Stack functions
void initStack(Stack *s) {
    s->top = -1;
}

void push(Stack *s, int value) {
    if (s->top < MAX_USERS - 1) {
        s->items[++(s->top)] = value;
    }
}

int pop(Stack *s) {
    if (s->top >= 0) {
        return s->items[(s->top)--];
    }
    return -1; // Error state
}

bool isStackEmpty(Stack *s) {
    return s->top == -1;
}

// Queue functions
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

void enqueue(Queue *q, int value) {
    if (q->rear == MAX_USERS - 1) {
        return; // Queue full
    }
    
    if (q->front == -1) {
        q->front = 0;
    }
    
    q->items[++(q->rear)] = value;
}

int dequeue(Queue *q) {
    if (q->front == -1 || q->front > q->rear) {
        return -1; // Queue empty
    }
    
    int value = q->items[q->front++];
    
    if (q->front > q->rear) {
        q->front = q->rear = -1;
    }
    
    return value;
}

bool isQueueEmpty(Queue *q) {
    return (q->front == -1 || q->front > q->rear);
}

// Red-Black Tree functions
RBNode* createNode(int user_id) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->user_id = user_id;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

void leftRotate(Graph *graph, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    
    if (y->left != NULL) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == NULL) {
        graph->rb_root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

void rightRotate(Graph *graph, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    
    if (x->right != NULL) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == NULL) {
        graph->rb_root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

void fixInsert(Graph *graph, RBNode *k) {
    RBNode *u;
    
    while (k != graph->rb_root && k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            
            if (u != NULL && u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(graph, k);
                }
                
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(graph, k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            
            if (u != NULL && u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(graph, k);
                }
                
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(graph, k->parent->parent);
            }
        }
    }
    
    graph->rb_root->color = BLACK;
}

void insertRB(Graph *graph, int user_id) {
    RBNode *node = createNode(user_id);
    RBNode *y = NULL;
    RBNode *x = graph->rb_root;
    
    while (x != NULL) {
        y = x;
        if (node->user_id < x->user_id) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    node->parent = y;
    
    if (y == NULL) {
        graph->rb_root = node;
    } else if (node->user_id < y->user_id) {
        y->left = node;
    } else {
        y->right = node;
    }
    
    if (node->parent == NULL) {
        node->color = BLACK;
        return;
    }
    
    if (node->parent->parent == NULL) {
        return;
    }
    
    fixInsert(graph, node);
}

// Search for a user in Red-Black Tree
int searchUserRB(RBNode *root, int user_id) {
    if (root == NULL) {
        return -1; // User not found
    }
    
    if (root->user_id == user_id) {
        return root->user_id;
    }
    
    if (user_id < root->user_id) {
        return searchUserRB(root->left, user_id);
    } else {
        return searchUserRB(root->right, user_id);
    }
}

// Find user by name
int findUserByName(Graph *graph, char *name) {
    for (int i = 0; i < graph->user_count; i++) {
        if (strcmp(graph->users[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // User not found
}

// Graph creation and initialization
void initGraph(Graph *graph) {
    graph->user_count = 0;
    graph->rb_root = NULL;
}

// Add new user
int addUser(Graph *graph, char *name) {
    if (graph->user_count >= MAX_USERS) {
        return -1; // Maximum user count reached
    }
    
    int user_id = graph->user_count;
    
    strcpy(graph->users[user_id].name, name);
    graph->users[user_id].id = user_id;
    graph->users[user_id].connection_count = 0;
    graph->users[user_id].influence_score = 0.0;
    graph->users[user_id].community_id = -1; // No community assigned initially
    
    // Add to Red-Black Tree
    insertRB(graph, user_id);
    
    return graph->user_count++;
}

// Create friendship connection between users (bidirectional)
bool createFriendship(Graph *graph, int user_id1, int user_id2) {
    if (user_id1 < 0 || user_id1 >= graph->user_count || 
        user_id2 < 0 || user_id2 >= graph->user_count || 
        user_id1 == user_id2) {
        return false;
    }
    
    // Check if connection already exists
    for (int i = 0; i < graph->users[user_id1].connection_count; i++) {
        if (graph->users[user_id1].connections[i] == user_id2) {
            return true; // Connection already exists
        }
    }
    
    // Add connection (both ways)
    if (graph->users[user_id1].connection_count < MAX_CONNECTIONS) {
        graph->users[user_id1].connections[graph->users[user_id1].connection_count++] = user_id2;
    }
    
    if (graph->users[user_id2].connection_count < MAX_CONNECTIONS) {
        graph->users[user_id2].connections[graph->users[user_id2].connection_count++] = user_id1;
    }
    
    return true;
}

// Find friends at specific distance
void findFriendsAtDistance(Graph *graph, int start_user_id, int distance) {
    if (start_user_id < 0 || start_user_id >= graph->user_count) {
        printf("Invalid user ID!\n");
        return;
    }
    
    int *distances = (int *)malloc(graph->user_count * sizeof(int));
    bool *visited = (bool *)malloc(graph->user_count * sizeof(bool));
    
    for (int i = 0; i < graph->user_count; i++) {
        distances[i] = -1;
        visited[i] = false;
    }
    
    // Calculate distances using BFS
    Queue q;
    initQueue(&q);
    enqueue(&q, start_user_id);
    visited[start_user_id] = true;
    distances[start_user_id] = 0;
    
    while (!isQueueEmpty(&q)) {
        int current = dequeue(&q);
        
        // If not at desired distance, continue
        if (distances[current] > distance) {
            continue;
        }
        
        // Check all connections of this user
        for (int i = 0; i < graph->users[current].connection_count; i++) {
            int neighbor = graph->users[current].connections[i];
            
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distances[neighbor] = distances[current] + 1;
                enqueue(&q, neighbor);
            }
        }
    }
    
    // Print friends exactly at the desired distance
    printf("Friends at distance %d from %s:\n", distance, graph->users[start_user_id].name);
    bool found = false;
    
    for (int i = 0; i < graph->user_count; i++) {
        if (distances[i] == distance) {
            printf("- %s (ID: %d)\n", graph->users[i].name, i);
            found = true;
        }
    }
    
    if (!found) {
        printf("No friends found at this distance.\n");
    }
    
    free(distances);
    free(visited);
}

// Find common friends between two users
void findCommonFriends(Graph *graph, int user_id1, int user_id2) {
    if (user_id1 < 0 || user_id1 >= graph->user_count ||
        user_id2 < 0 || user_id2 >= graph->user_count) {
        printf("Invalid user ID!\n");
        return;
    }
    
    printf("Common friends between %s and %s:\n", 
           graph->users[user_id1].name, 
           graph->users[user_id2].name);
    
    int common_count = 0;
    
    // Check if each friend is common
    for (int i = 0; i < graph->users[user_id1].connection_count; i++) {
        int friend1 = graph->users[user_id1].connections[i];
        
        for (int j = 0; j < graph->users[user_id2].connection_count; j++) {
            if (friend1 == graph->users[user_id2].connections[j]) {
                printf("- %s (ID: %d)\n", graph->users[friend1].name, friend1);
                common_count++;
                break;
            }
        }
    }
    
    if (common_count == 0) {
        printf("No common friends found.\n");
    } else {
        printf("Total %d common friend(s) found.\n", common_count);
    }
}

// Calculate influence - determine how influential a user is
void calculateInfluence(Graph *graph) {
    // Calculate a simple influence score for each user
    // Initially based on connection count and second-degree connections
    
    for (int i = 0; i < graph->user_count; i++) {
        // Score for direct connections
        float score = graph->users[i].connection_count;
        
        // Add score for second-degree connections with lower weight
        int second_degree = 0;
        for (int j = 0; j < graph->users[i].connection_count; j++) {
            int friend_id = graph->users[i].connections[j];
            second_degree += graph->users[friend_id].connection_count;
        }
        
        score += (float)second_degree * 0.1; // Lower weight for second-degree connections
        
        graph->users[i].influence_score = score;
    }
    
    // Sort to find most influential users
    int *sorted_indices = (int *)malloc(graph->user_count * sizeof(int));
    for (int i = 0; i < graph->user_count; i++) {
        sorted_indices[i] = i;
    }
    
    // Simple bubble sort
    for (int i = 0; i < graph->user_count - 1; i++) {
        for (int j = 0; j < graph->user_count - i - 1; j++) {
            if (graph->users[sorted_indices[j]].influence_score < 
                graph->users[sorted_indices[j + 1]].influence_score) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    // Print most influential users
    printf("Most influential users:\n");
    int display_count = graph->user_count > 10 ? 10 : graph->user_count;
    
    for (int i = 0; i < display_count; i++) {
        int idx = sorted_indices[i];
        printf("%d. %s (ID: %d) - Influence Score: %.2f\n", 
               i + 1, graph->users[idx].name, idx, graph->users[idx].influence_score);
    }
    
    free(sorted_indices);
}

// Community detection (using simple Union-Find algorithm)
void detectCommunities(Graph *graph) {
    // Parent array for Union-Find structure
    int *parent = (int *)malloc(graph->user_count * sizeof(int));
    
    // Initially each node is in its own community
    for (int i = 0; i < graph->user_count; i++) {
        parent[i] = i;
    }
    
    // Find operation - find the root node (community) of a node
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }
    
    // Union operation - merge two communities
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX != rootY) {
            parent[rootY] = rootX;
        }
    }
    
    // Apply union operation for all connections
    for (int i = 0; i < graph->user_count; i++) {
        for (int j = 0; j < graph->users[i].connection_count; j++) {
            unite(i, graph->users[i].connections[j]);
        }
    }
    
    // Calculate communities
    for (int i = 0; i < graph->user_count; i++) {
        graph->users[i].community_id = find(i);
    }
    
    // Calculate number of users in each community
    int *community_sizes = (int *)calloc(graph->user_count, sizeof(int));
    for (int i = 0; i < graph->user_count; i++) {
        community_sizes[graph->users[i].community_id]++;
    }
    
    // Print communities
    printf("Detected communities:\n");
    bool found = false;
    
    for (int i = 0; i < graph->user_count; i++) {
        if (community_sizes[i] > 0) {
            printf("Community %d (%d members):\n", i, community_sizes[i]);
            
            for (int j = 0; j < graph->user_count; j++) {
                if (graph->users[j].community_id == i) {
                    printf("- %s (ID: %d)\n", graph->users[j].name, j);
                }
            }
            printf("\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("No communities detected.\n");
    }
    
    free(parent);
    free(community_sizes);
}

// Print user information
void printUserInfo(Graph *graph, int user_id) {
    if (user_id < 0 || user_id >= graph->user_count) {
        printf("Invalid user ID!\n");
        return;
    }
    
    User *user = &graph->users[user_id];
    
    printf("User Information:\n");
    printf("ID: %d\n", user->id);
    printf("Name: %s\n", user->name);
    printf("Number of Friends: %d\n", user->connection_count);
    printf("Influence Score: %.2f\n", user->influence_score);
    printf("Community ID: %d\n", user->community_id);
    
    printf("Friends:\n");
    if (user->connection_count == 0) {
        printf("- No friends yet.\n");
    } else {
        for (int i = 0; i < user->connection_count; i++) {
            int friend_id = user->connections[i];
            printf("- %s (ID: %d)\n", graph->users[friend_id].name, friend_id);
        }
    }
}

// Memory cleanup
void freeRBNode(RBNode *node) {
    if (node != NULL) {
        freeRBNode(node->left);
        freeRBNode(node->right);
        free(node);
    }
}

void freeGraph(Graph *graph) {
    freeRBNode(graph->rb_root);
}

// Print all users in the network
void printAllUsers(Graph *graph) {
    printf("\nAll Users in the Network:\n");
    for (int i = 0; i < graph->user_count; i++) {
        printf("%d. %s (ID: %d)\n", i+1, graph->users[i].name, i);
    }
    printf("\n");
}

// Interactive menu for user input
void showMenu() {
    printf("\n===== Social Network Analysis Menu =====\n");
    printf("1. Add a new user\n");
    printf("2. Create friendship connection between users\n");
    printf("3. Find friends at specific distance\n");
    printf("4. Find common friends between users\n");
    printf("5. Calculate influence scores\n");
    printf("6. Detect communities\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

// Main function
int main() {
    Graph graph;
    initGraph(&graph);
    int choice, user_id1, user_id2, distance;
    char name[MAX_NAME_LENGTH];
    
    printf("===== Social Network Analysis Program =====\n\n");
    
    while (1) {
        showMenu();
        scanf("%d", &choice);
        getchar(); // Clear input buffer
        
        switch (choice) {
            case 1: // Add user
                printf("Enter user name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0; // Remove newline
                
                int new_id = addUser(&graph, name);
                if (new_id >= 0) {
                    printf("User '%s' added successfully with ID: %d\n", name, new_id);
                } else {
                    printf("Error: Could not add user. Maximum user limit reached.\n");
                }
                break;
                
            case 2: // Create friendship
                printAllUsers(&graph);
                
                printf("Enter first user's ID or name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0;
                
                // Check if input is numeric (ID) or name
                if (atoi(name) != 0 || name[0] == '0') {
                    user_id1 = atoi(name);
                } else {
                    user_id1 = findUserByName(&graph, name);
                    if (user_id1 == -1) {
                        printf("Error: User '%s' not found.\n", name);
                        break;
                    }
                }
                
                printf("Enter second user's ID or name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0;
                
                if (atoi(name) != 0 || name[0] == '0') {
                    user_id2 = atoi(name);
                } else {
                    user_id2 = findUserByName(&graph, name);
                    if (user_id2 == -1) {
                        printf("Error: User '%s' not found.\n", name);
                        break;
                    }
                }
                
                if (createFriendship(&graph, user_id1, user_id2)) {
                    printf("Friendship created between %s and %s\n", 
                           graph.users[user_id1].name, graph.users[user_id2].name);
                } else {
                    printf("Error: Could not create friendship. Check user IDs.\n");
                }
                break;
                
            case 3: // Find friends at distance
                printAllUsers(&graph);
                printf("Enter starting user ID or name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0;
                
                if (atoi(name) != 0 || name[0] == '0') {
                    user_id1 = atoi(name);
                } else {
                    user_id1 = findUserByName(&graph, name);
                    if (user_id1 == -1) {
                        printf("Error: User '%s' not found.\n", name);
                        break;
                    }
                }
                
                printf("Enter distance (1 for direct friends, 2 for friends of friends, etc.): ");
                scanf("%d", &distance);
                getchar(); // Clear input buffer
                
                findFriendsAtDistance(&graph, user_id1, distance);
                break;
                
            case 4: // Find common friends
                printAllUsers(&graph);
                printf("Enter first user ID or name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0;
                
                if (atoi(name) != 0 || name[0] == '0') {
                    user_id1 = atoi(name);
                } else {
                    user_id1 = findUserByName(&graph, name);
                    if (user_id1 == -1) {
                        printf("Error: User '%s' not found.\n", name);
                        break;
                    }
                }
                
                printf("Enter second user ID or name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0;
                
                if (atoi(name) != 0 || name[0] == '0') {
                    user_id2 = atoi(name);
                } else {
                    user_id2 = findUserByName(&graph, name);
                    if (user_id2 == -1) {
                        printf("Error: User '%s' not found.\n", name);
                        break;
                    }
                }
                
                findCommonFriends(&graph, user_id1, user_id2);
                break;
                
            case 5: // Calculate influence
                calculateInfluence(&graph);
                break;
                
            case 6: // Detect communities
                detectCommunities(&graph);
                break;
                
            case 7: // Exit
                printf("Exiting program. Goodbye!\n");
                freeGraph(&graph);
                return 0;
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
