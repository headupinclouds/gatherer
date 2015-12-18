#import <OpenGLES/EAGL.h>
 
void * currentEAGLContext()
{
	return [EAGLContext currentContext];
}
