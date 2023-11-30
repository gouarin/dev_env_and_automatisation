# Faciliter l'ajout de code

Dans le processus de développement, l'ajout de code au dépôt principal via des pull request a une place importante. Nous ne souhaitons pas que chacun des contributeurs mettent leur style d'écriture dans les fichiers. Nous appelons ici **style d'écriture** tout ce qui se rapporte au formatage d'un fichier : les espaces, les indentations, les règles de nommage, ... Même si nous voulons un cadre le moins contraint possible pour laisser une grande part à la créativité, il est néanmoins nécessaire de mettre quelques règles pour que ce ne soit pas le bazar.

Un autre point important est de pouvoir se focaliser sur l'essentiel. Lors de la relecture d'une pull request, vous n'allez certainement pas être capable de traiter les problèmes de formatage, les problèmes d'algorithmie, les bugs cachés, ... Il est donc important d'automatiser tout ce qui peu l'être en amont.

## Le formatage du code

Dans chaque langage, il existe des outils qui permettent de formater le texte selon un fichier de configuration. Voici quelques exemples suivant le langage de programmation

- Python : [black](https://black.readthedocs.io), [ruff](https://docs.astral.sh/ruff/)
- C++ : [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- Julia : [JuliaFormatter.jl](https://domluna.github.io/JuliaFormatter.jl/dev/)
- Rust : [Rustfmt](https://rust-lang.github.io/rustfmt)

Le fichier de configuration qui explique comment le code source doit être formaté se trouve à la racine de votre projet. Pour l'utilisation de clang-format, il vous faut créer un fichier `.clang-format`. Nous ne rentrerons pas ici dans sa configuration. Vous pouvez trouver sur internet différents outils en ligne comme [clang format configurator](https://zed0.co.uk/clang-format-configurator/). Regardons plutôt ce que cela fait sur un exemple concret. Prenons ce code source non formaté

```cpp
#include <iostream>
#include<vector>


int Func( std::vector<double>& a ){
    for(std::size_t i=0; i <10; )
    {
        a[ i] =1.;
    }

}

int main()
{
 Func( std::vector<double>(10) );
}
```

et le code formaté selon un style défini

```cpp
#include <iostream>
#include <vector>

int Func(std::vector<double>& a)
{
    for (std::size_t i = 0; i < 10;)
    {
        a[i] = 1.;
    }
}

int main()
{
    Func(std::vector<double>(10));
}
```

Vous pouvez aller plus loin et définir les règles de nommage pour vos classes, vos fonctions, ... Nous avons ici `Func` qui est en `CamelCase` mais vous préféreriez peut-être plus le `lower_case`. Ces outils permettent également de le faire. Pour le C++, il faut aller voir du côté de [clang-tidy](https://clang.llvm.org/extra/clang-tidy/).

Mais, il y a pire dans cet exemple : le code est complètement bugué et, s'il n'y a pas de tests, vous pourriez passer à côté même si nous avons ici grossi le trait. Nous allons voir dans la prochaine partie comment limiter la casse.

## L'analyse statique

En plus du formatage, vous avez des outils qui vont regarder la syntaxe de votre code sans rien compiler ou exécuter. Là encore, vous en trouverez au moins un pour votre langage préféré

- Python : [pylint](https://pylint.readthedocs.io/) et [ruff](https://docs.astral.sh/ruff/)
- C++ : [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
- Julia : [Lint.jl](https://lintjl.readthedocs.io)
- Rust : [Clippy](https://doc.rust-lang.org/clippy/)

Si nous utilisons clang-tidy sur notre exemple précédent voici ce que nous obtenons

```shell
clang-tidy -checks='-*,bugprone-infinite-loop,readability-identifier-naming' \
           -config="{CheckOptions: [ {key: readability-identifier-naming.FunctionCase, value: lower_case} ]}" \
           tests/try-format.cpp
```

```log
1144 warnings and 1 error generated.
Error while processing /Users/.../tests/try-format.cpp.
/Users/.../tests/try-format.cpp:5:5: warning: invalid case style for function 'Func' [readability-identifier-naming]
int Func( std::vector<double>& a ){
    ^~~~
    func
/Users/.../tests/try-format.cpp:6:5: warning: this loop is infinite; none of its condition variables (i) are updated in the loop body [bugprone-infinite-loop]
    for(std::size_t i=0; i <10; )
    ^
/Users/.../tests/try-format.cpp:15:2: error: no matching function for call to 'Func' [clang-diagnostic-error]
 Func( std::vector<double>(10) );
 ^~~~
/Users/.../tests/try-format.cpp:5:5: note: candidate function not viable: expects an lvalue for 1st argument
int Func( std::vector<double>& a ){
    ^
Suppressed 1142 warnings (1140 in non-user code, 2 with check filters).
Use -header-filter=.* to display errors from all non-system headers. Use -system-headers to display errors from system headers as well.
Found compiler error(s).
```

La mise en place de ces outils vous fera gagner un temps inestimable et vous fera monter en compétences ainsi que votre communauté. En effet, en fonction des dysfonctionnements remontés, vous allez vous habituer et vous serez beaucoup plus vigilant lors de l'écriture du code.