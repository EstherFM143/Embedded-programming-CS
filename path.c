#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point {
    int coordinates[3];
    struct point *next;
};

struct path {
    double distance;
    struct point *points;
};

double calculate_distance(struct point *p1, struct point *p2);

double calculate_distance(struct point *p1, struct point *p2) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
        double diff = p1->coordinates[i] - p2->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

void calculate_path_distance(struct path *path);

void calculate_path_distance(struct path *path) {
    if (path == NULL || path->points == NULL) {
        return;
    }

    path->distance = 0;
    struct point *current = path->points;
    struct point *next = current->next;

    while (next != NULL) {
        path->distance += calculate_distance(current, next);
        current = next;
        next = next->next;
    }
}

int main() {
    // Create a path with three points: (1,2,3), (3,4,6), (4,4,4)
    struct point *p1 = malloc(sizeof(struct point));
    struct point *p2 = malloc(sizeof(struct point));
    struct point *p3 = malloc(sizeof(struct point));

    p1->coordinates[0] = 1; p1->coordinates[1] = 2; p1->coordinates[2] = 3;
    p2->coordinates[0] = 3; p2->coordinates[1] = 4; p2->coordinates[2] = 6;
    p3->coordinates[0] = 4; p3->coordinates[1] = 4; p3->coordinates[2] = 4;

    p1->next = p2;
    p2->next = p3;
    p3->next = NULL;

    struct path test_path = {0, p1};

    calculate_path_distance(&test_path);

    printf("Total path distance: %f\n", test_path.distance);

    // Free allocated memory
    free(p1);
    free(p2);
    free(p3);

    return 0;
}
