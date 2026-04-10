default:
	gcc src/main.c src/util.c src/level.c src/player.c src/rendering.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Llib -Iinclude -o platformer

