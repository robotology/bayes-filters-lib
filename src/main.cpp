#include "FilteringContext.h"
#include "SIRParticleFilter.h"

int main()
{
    FilteringContext fc(new SIRParticleFilter);

    fc.run();

    fc.saveResult();
    
    return EXIT_SUCCESS;
}
