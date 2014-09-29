CC=		gcc

#CFLAGS=		-Wall -Wextra -O0 -ggdb -std=gnu99
#DFLAGS=		-DPRGDEBUG
CFLAGS=		-Wall -Wextra -O3 -fomit-frame-pointer -ffast-math -std=gnu99

PROG=		annsim
SOLVER=		timer.c util.c types.c graph.c hash_map.c\
		CA.c metric.c ann_sim.c input.c main.c
BLOSSOM= 	mat_main.c match.c matprice.c

SOLVEROBJS=	$(SOLVER:.c=.o)
BLOSSOMOBJS=    $(BLOSSOM:.c=.o)
CONC_PATH=	lib/concorde970827
INCLUDES=	-I. -I$(CONC_PATH)
LIBS=		-lm
INSTALLDIR=	../

.SUFFIXES:.c .o

all:
		@echo
		@echo " ********************************** "
		@echo " *** Compiling CONCORDE Solver  *** "
		@echo " ********************************** "
		@echo
		(cd $(CONC_PATH) ; $(MAKE))
		@echo
		@echo " ********************************** "
		@echo " *** Compiling  Metric  Solver  *** "
		@echo " ********************************** "
		@echo
		($(MAKE) solver)

solver:		$(PROG)

$(PROG):	$(SOLVEROBJS) $(BLOSSOMOBJS)
		$(CC) $(CFLAGS) $(GVFLAGS)  -o $(INSTALLDIR)$(PROG) $(BLOSSOMOBJS) $(SOLVEROBJS)  $(CONC_PATH)/concorde.a $(LIBS) $(LDFLAGS)

.c.o:
	$(CC) -c $(INCLUDES) $(CFLAGS) $(DFLAGS) $(GVFLAGS) $(MACGVFLAGS) $< -o $@

.PHONY : clean

clean :
	rm -rf $(INSTALLDIR)$(PROG) *.o *.dSYM *~

veryclean:
	$(MAKE) clean
	(cd $(CONC_PATH) ; $(MAKE) clean)
