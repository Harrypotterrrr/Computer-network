# s指static，d指dynamic
CFLAGS_s = -static $$(mysql_config --cflags) -L/usr/lib64/mysql -lmysqlclient -lpthread -ldl
CFLAGS_d = $$(mysql_config --cflags --libs)
CC = g++
FILE = mysql_demo

# 此为静态编译，生成mysql_demo
$(FILE): $(FILE).cpp
	$(CC) -o $(FILE) $(FILE).cpp $(CFLAGS_s)

.PHONY = clean $(FILE)_d

# 此为动态编译，生成mysql_demo_d
$(FILE)_d: $(FILE).cpp
	$(CC) -o $(FILE)_d $(FILE).cpp $(CFLAGS_d)

clean:
	rm -f $(FILE) $(FILE)_d

