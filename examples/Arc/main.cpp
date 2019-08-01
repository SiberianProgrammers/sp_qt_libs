#include <SpApplicationPrototype.h>

int main(int argc, char *argv[])
{
    sp::SpApplicationPrototype app(argc, argv, "Arc Example");

    return app.exec();
}
