#ifndef L_RESOURCE_MANAGER_H
#define L_RESOURCE_MANAGER_H

#include "liResourceManager.h"

#include <string>

typedef liResourceManager* (*lfptrCreateResourceManager) (lmScalar *def_diffuse,lmScalar *def_specular,lmScalar def_shininess);
typedef bool (*lfptrLoadText)(const std::string &filename,std::string &destination);
/*
extern "C" {

liResourceManager *lrmCreateResourceManager(lmScalar *def_diffuse,lmScalar *def_specular,lmScalar def_shininess);


//liResourceManager *(*lfptrCreateResourceManager) (lmScalar *def_diffuse,lmScalar *def_specular,lmScalar def_shininess);

/** \brief Szövegbetöltő függvény, amely egy fájlból a paraméterbe kapott stringbe betölt egy szöveget.
 *
 * \param filename const std::string&: a beolvasandó fájl neve
 * \param Target std::string&: a string, amelybe a fájlt beolvassuk.
 * \return bool - true, ha a fájl megnyitása sikeres, false egyébként.
 *
 * /
bool lrmLoadText(const std::string &filename,std::string &destination);

}
*/
#endif // L_RESOURCE_MANAGER_H
