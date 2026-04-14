#ifndef STATE_H
#define STATE_H

typedef enum State {
    EDITOR,
    PLAYING,
} State;

extern State state;

void state_editor_enter(void);
void state_playing_enter(void);
void state_update(void);

#endif
