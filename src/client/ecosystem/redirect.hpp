#ifndef __REDIRECT_HPP
#define __REDIRECT_HPP

class StdOutRedirect {
    public:
		bool initialized = false;
        StdOutRedirect();
        ~StdOutRedirect();
		void initialize();
        int Start();
        int Stop();
        int GetBuffer(char *buffer);

    private:
        int fdStdOutPipe[2];
        int fdStdOut;
};

#endif 