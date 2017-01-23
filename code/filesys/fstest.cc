// fstest.cc 
//	Simple test routines for the file system.  
//
//	We implement:
//	   Copy -- copy a file from UNIX to Nachos
//	   Print -- cat the contents of a Nachos file 
//	   Perftest -- a stress test for the Nachos file system
//		read and write a really large file in tiny chunks
//		(won't work on baseline system!)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "utility.h"
#include "filesys.h"
#include "system.h"
#include "thread.h"
#include "disk.h"
#include "stats.h"

#define TransferSize 	10 	// make it small, just to be difficult

//----------------------------------------------------------------------
// Copy
// 	Copy the contents of the UNIX file "from" to the Nachos file "to"
//----------------------------------------------------------------------

void
Copy(const char *from, const char *to)
{
    FILE *fp;
    OpenFile* openFile;
    int amountRead, fileLength;
    char *buffer;

// Open UNIX file
    if ((fp = fopen(from, "r")) == NULL) {	 
	printf("Copy: couldn't open input file %s\n", from);
	return;
    }

// Figure out length of UNIX file
    fseek(fp, 0, 2);		
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

// Create a Nachos file of the same length
    DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
    if (!fileSystem->Create(to, fileLength)) {	 // Create Nachos file
	printf("Copy: couldn't create output file %s\n", to);
	fclose(fp);
	return;
    }
    
    openFile = fileSystem->Open(to);
    ASSERT(openFile != NULL);
    
// Copy the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
	openFile->Write(buffer, amountRead);	
    delete [] buffer;

// Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}

//----------------------------------------------------------------------
// Print
// 	Print the contents of the Nachos file "name".
//----------------------------------------------------------------------

void
Print(char *name)
{
    OpenFile *openFile;    
    int i, amountRead;
    char *buffer;

    if ((openFile = fileSystem->Open(name)) == NULL) {
	printf("Print: unable to open file %s\n", name);
	return;
    }
    
    buffer = new char[TransferSize];
    while ((amountRead = openFile->Read(buffer, TransferSize)) > 0)
	for (i = 0; i < amountRead; i++)
	    printf("%c", buffer[i]);
    delete [] buffer;

    delete openFile;		// close the Nachos file
    return;
}

//----------------------------------------------------------------------
// PerformanceTest
// 	Stress the Nachos file system by creating a large file, writing
//	it out a bit at a time, reading it back a bit at a time, and then
//	deleting the file.
//
//	Implemented as three separate routines:
//	  FileWrite -- write the file
//	  FileRead -- read the file
//	  PerformanceTest -- overall control, and print out performance #'s
//----------------------------------------------------------------------

#define FileName 	"TestFile"
#define Contents 	"1234567890"
#define ContentSize 	strlen(Contents)
#define FileSize 	((int)(ContentSize * 5000))

static void 
FileWrite()
{
    OpenFile *openFile;    
    int i, numBytes;

    printf("Sequential write of %d byte file, in %zd byte chunks\n", 
	FileSize, ContentSize);
    if (!fileSystem->Create(FileName, 0)) {
      printf("Perf test: can't create %s\n", FileName);
      return;
    }
    openFile = fileSystem->Open(FileName);
    if (openFile == NULL) {
	printf("Perf test: unable to open %s\n", FileName);
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Write(Contents, ContentSize);
	if (numBytes < 10) {
	    printf("Perf test: unable to write %s\n", FileName);
	    delete openFile;
	    return;
	}
    }
    delete openFile;	// close file
}

static void 
FileRead()
{
    OpenFile *openFile;    
    char *buffer = new char[ContentSize];
    int i, numBytes;

    printf("Sequential read of %d byte file, in %zd byte chunks\n", 
	FileSize, ContentSize);

    if ((openFile = fileSystem->Open(FileName)) == NULL) {
	printf("Perf test: unable to open file %s\n", FileName);
	delete [] buffer;
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Read(buffer, ContentSize);
	if ((numBytes < 10) || strncmp(buffer, Contents, ContentSize)) {
	    printf("Perf test: unable to read %s\n", FileName);
	    delete openFile;
	    delete [] buffer;
	    return;
	}
    }
    delete [] buffer;
    delete openFile;	// close file
}

void
PerformanceTest()
{
    printf("Starting file system performance test:\n");
    stats->Print();
    FileWrite();
    FileRead();
    if (!fileSystem->Remove(FileName)) {
      printf("Perf test: unable to remove %s\n", FileName);
      return;
    }
    stats->Print();
}

#ifdef CHANGED
void Terminal()
{
    
    Directory* tempDirectory;
    char* input = new char[MAX_STRING_SIZE];
    char* path  = new char[MAX_STRING_SIZE];
    strcpy(path, "/");
    tempDirectory= fileSystem->GetRootDirectory();
    synchconsole->SynchPutString("Terminal Initiated\n");
    while(1)
    {
        fprintf(stderr, "PNG: $"    );
        synchconsole->SynchGetString(input, MAX_STRING_SIZE);
        if(!strncmp(input, "q", 1))
            break;
        
        
        if (!strncmp(input, "ls", 2))
        {
           
             if (tempDirectory == NULL)
             {
                fprintf(stderr, "Directory %s not found\n",path );
                fprintf(stderr, "Reinitialized to root \n");
                tempDirectory= fileSystem->GetRootDirectory();
                continue;
             }

             tempDirectory->List();
             //delete tempPath;


        }
        else if (!strncmp(input, "mkdir", 5))
        {
            
            path = input;
            while(*path != ' '&&*path)
                path++;
            
            if (path == '\0')
             {
                synchconsole->SynchPutString("unspecified path\n");
                continue;
             }
             path ++;
            if(*path=='/')
             {
                if(!fileSystem->CreateDirectory (fileSystem->GetRootDirectory(), path))
                {
                    fprintf(stderr, "Error in creating \n" );
                }
             }
            else  if(!fileSystem->CreateDirectory (tempDirectory,path))
                {
                    fprintf(stderr, "Error in creating \n" );
                }
            
            tempDirectory= fileSystem-> GetRootDirectory();
            fprintf(stderr, "Path reinitialized to root directory /\n" );

        }
        else if (!strncmp(input, "rmdir", 5))
        {
            path = input;
            while(*path != ' '&&*path)
                path++;
            
            if (path == '\0')
             {
                synchconsole->SynchPutString("unspecified path\n");
                continue;
             }
             path ++;
             if(*path=='/')
             {
                if(!fileSystem->RemoveDirectory (fileSystem->GetRootDirectory(), path))
                {
                    fprintf(stderr, "Error in removing \n" );
                }
             }
            else  if(!fileSystem->RemoveDirectory (tempDirectory,path))
                {
                    fprintf(stderr, "Error in removing \n" );
                }
            tempDirectory= fileSystem-> GetRootDirectory();
            fprintf(stderr, "Path reinitialized to root directory /\n" );
            
        }
        else if (!strncmp(input, "cd", 2))
        {
            path = input;
            while(*path && *path!=' ')
                path++;
            if (*path == '\0')
             {
                synchconsole->SynchPutString("unspecified path\n");
                continue;
             }  
             path++;


            Directory* temp = tempDirectory;
            if(*path=='/')
             {
                temp =  fileSystem->FindDirectory(fileSystem->GetRootDirectory(), path);
             }
            else 
            {
                temp =  fileSystem->FindDirectory(temp, path);
            }
            
            if (temp==NULL)
                fprintf(stderr, "Directory %s not found \n",path );
            else 
            {

                tempDirectory = temp;
                
            }
         

        }




    }
}

#endif