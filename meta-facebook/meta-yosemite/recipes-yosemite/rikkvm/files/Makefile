all: rikkvm 

rikkvm: kvm_main.cpp ikvm_args.cpp ikvm_manager.cpp ikvm_input.cpp ikvm_video.cpp 
	$(CXX) $(CXXFLAGS) -pthread -std=c++11 -o $@ $^ -I$(SINC)/glib-2.0 -I$(SLIB)/glib-2.0
.PHONY: clean

clean:
	rm -rf *.o rikkvm


