#include <SpApplicationPrototype.h>

int main(int argc, char *argv[])
{
    sp::SpApplicationPrototype app(argc, argv, "ImageSp Example");

    return app.exec();
}
