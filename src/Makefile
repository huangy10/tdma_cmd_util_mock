# http://scottmcpeak.com/autodepend/autodepend.html
# http://wiki.ubuntu.org.cn/%E8%B7%9F%E6%88%91%E4%B8%80%E8%B5%B7%E5%86%99Makefile:%E4%B9%A6%E5%86%99%E8%A7%84%E5%88%99
objs := \
 	undefined.o \
	tdma_mock.o

undefined: $(objs)
	$(CC) -o undefined undefined.o tdma_mock.o
	# $(CC) -o tdma_route_ls tdma_route_ls.o tdma_mock.o

-include $(objs:.o=.d) 

%.o: %.c
	$(CC) -c $(CPPFLAGS) $*.c -o $*.o
	@$(CC) -MM $(CPPFLAGS) $*.c > $*.d 

.PHONY : clean
clean:
	@rm -f *.d *.o
	@rm -f undefined
