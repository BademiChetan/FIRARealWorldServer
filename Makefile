# Simple Makefile

# Your compiler
CC = g++
# Compilation flags
# '-g' turns debugging flags on.
CFLAGS = `pkg-config opencv libserial --cflags --libs` -lboost_thread -lpthread

# Linker flags
# When you need to add a library
# '-lm' adds the math library
LDFLAGS =   -lncurses

# all is a target. reversewords is a 'dependency' or requirement for all to be complete
all: main 


# The main program depends on the complex number library, and the main file
main: main.o  kick_off_calibration.o select_arena.o colors.o our_bot.o opp_bot.o ball.o update_frame.o process_image.o elec.o contours.o algo.o bot_actions.o 
	$(CC) -g -o $@ $^ $(CFLAGS)

main.o : main.cpp global_var.h
	$(CC) -g -c $< $(CFLAGS)
	
select_arena.o : select_arena.cpp
	$(CC) -g -c $< $(CFLAGS)

process_image.o : process_image.cpp
	$(CC) -g -c $< $(CFLAGS)

elec.o : elec.cpp
	$(CC) -g -c $< $(CFLAGS)

contours.o : contours.cpp process_image.o global_var.h
	$(CC) -g -c $< $(CFLAGS)

colors.o: colors.cpp process_image.o contours.o global_var.h
	$(CC) -g -c $< $(CFLAGS) 			

bot_actions.o: bot_actions.cpp elec.o global_var.h
	$(CC) -g -c $< $(CFLAGS) 			

our_bot.o: our_bot.cpp process_image.o colors.o global_var.h
	$(CC) -g -c $< $(CFLAGS) 			

opp_bot.o : opp_bot.cpp process_image.o colors.o contours.o global_var.h
	$(CC) -g -c $< $(CFLAGS)

ball.o : ball.cpp process_image.o colors.o contours.o global_var.h
	$(CC) -g -c $< $(CFLAGS)

update_frame.o: update_frame.cpp process_image.o our_bot.o opp_bot.o ball.o global_var.h
	$(CC) -g -c $< $(CFLAGS) 

algo.o: algo.cpp elec.cpp our_bot.cpp ball.cpp opp_bot.cpp global_var.h
	$(CC) -g -c $< $(CFLAGS)

kick_off_calibration.o: kick_off_calibration.cpp elec.cpp our_bot.cpp process_image.cpp update_frame.cpp global_var.h
	$(CC) -g -c $< $(CFLAGS)

# .PHONY tells make that 'all' or 'clean' aren't _actually_ files, and always
# execute the compilation action when 'make all' or 'make clean' are used
.PHONY: all clean							

# Delete all the temporary files we've created so far
clean:
	rm -rf *.o  main


