# Faciliter l'ajout de code

Dans le processus de développement, l'ajout de code au dépôt principal via des pull request a une place importante. Nous ne souhaitons pas que chacun des contributeurs mettent leur style d'écriture dans les fichiers. Nous appelons ici **style d'écriture** tout ce qui se rapporte au formatage d'un fichier : les espaces, les indentations, les règles de nommage, ... Même si nous voulons un cadre le moins contraint possible pour laisser une grande part à la créativité, il est néanmoins nécessaire de mettre quelques règles pour que ce ne soit pas le bazar.

Un autre point important est de pouvoir se focaliser sur l'essentiel. Lors de la relecture d'une pull request, vous n'allez certainement pas être capable de traiter les problèmes de formatage, les problèmes d'algorithmie, les bugs cachés, ... Il est donc important d'automatiser tout ce qui peu l'être en amont.

## Le formatage du code

Dans chaque langage, il existe des outils qui permettent de formater le texte selon un fichier de configuration. Voici quelques exemples suivant le langage de programmation

- Python : [black](https://black.readthedocs.io), [ruff](https://docs.astral.sh/ruff/)
- C++ : [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- Julia : [JuliaFormatter.jl](https://domluna.GitHub.io/JuliaFormatter.jl/dev/)
- Rust : [Rustfmt](https://rust-lang.GitHub.io/rustfmt)

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

La mise en place de ces outils vous fera gagner un temps inestimable et vous fera monter en compétences ainsi que votre communauté. En effet, en fonction des dysfonctionnements remontés, vous allez vous habituer à observer et à les identifier. Vous serez beaucoup plus vigilant lors de l'écriture du code.

Il nous reste un élément indispensable à mettre en œuvre. Jusqu'à présent, nous avons présenté deux outils, mais vous pouvez toujours ne pas les lancer sur vos modifications. Comment s'assurer alors que nous n'avons pas fait ça pour rien ? Comment s'assurer que ces outils maintiennent notre projet en bonne santé ? Il faut probablement automatiser, mais à quel moment ? Lors des tests sur la pull request fait par l'intégration continue ? Il est déjà trop tard !

Et, si nous pouvions le faire localement à chaque commit ? C'est l'objet de la prochaine section.

## pre-commit

L'outil [pre-commit](https://pre-commit.com/) va nous permettre d'automatiser l'utilisation des deux outils présentés plus haut et à chaque commit. Il permet de créer des *hooks* git de manière très simple et il est facilement extensible. Vous pouvez aller voir la liste des [hooks](https://pre-commit.com/hooks.html) et trouver votre bonheur.

:::{note} Remarque
Nous nous intéressons ici à un projet écrit en C++ mais il vous est bien évidemment possible d'utiliser pre-commit pour d'autres langages
:::

Commençons par son installation. Nous allons rester dans l'univers de pixi et donc faire

```shell
pixi install pre-commit
```

Vous pouvez à présent initier pre-commit à l'aide de la ligne de commande suivante

```bash
pre-commit install
```

:::{attention}
Si un contributeur ne fait pas le `pre-commit install` localement les hooks ne seront pas installés et donc, aucun test ne sera effectué. Il est important de bien documenter cette étape pour qu'elle rentre dans les habitudes et intervienne juste après le clonage du dépôt.
:::

Il nous faut à présent définir ce que nous voulons tester à chaque commit en écrivant un fichier de configuration appelé `.pre-commit-config.yaml`. C'est bien évidemment à vous de mesurer l'impact de l'utilisation de cet outil. Si vous êtes trop contraignant et que le pre-commit prend une minute, vous n'aurez probablement pas l'adhésion des contributeurs. Il vous faudra donc trouver une solution acceptable qui profite à tous.

Dans le cadre de cet atelier, nous souhaitons vérifier la liste suivante à chaque commit

- ne pas mettre des fichiers trop gros (`check-added-large-files`)
- ne pas donner des noms de fichiers ou de répertoires qui ne sont pas compatibles avec tous les OS (`check-case-conflict`)
- vérifier que les fichiers `json` et `yaml` sont bien construits (`check-json`, `check-yaml`)
- faire en sorte que tous les fichiers finissent par une ligne vide (`end-of-file-fixer`)
- enlever les espaces qui ne servent à rien (`trailing-whitespace`)
- vérifier qu'il n'y a pas de fichiers qui sont en conflit de fusion (`check-merge-conflict`)
- vérifie que l'on n'essaie pas de pousser dans la branche principale (`no-commit-to-branch`)
- vérifier les caractères de fin de lignes et les rendre homogènes (`mixed-line-ending`)
- remplacer les tabulations par des espaces (https://GitHub.com/Lucas-C/pre-commit-hooks)
- vérifier que les changements sont bien compatibles avec le formatage souhaité décrit dans le fichier `.clang-format` (https://GitHub.com/pre-commit/mirrors-clang-format)

Voici à quoi ressemble le fichier `.pre-commit-config.yaml`

```yaml
repos:
  - repo: https://GitHub.com/pre-commit/pre-commit-hooks
    rev: v4.4.0
    hooks:
      - id: check-added-large-files
      - id: check-case-conflict
      - id: end-of-file-fixer
      - id: trailing-whitespace
      - id: check-merge-conflict
      - id: check-yaml
        exclude: conda/recipe/meta.yaml
      - id: check-json
      - id: no-commit-to-branch
      - id: mixed-line-ending
        args: ["--fix=lf"]
        description: Forces to replace line ending by the UNIX 'lf' character.
  - repo: https://GitHub.com/Lucas-C/pre-commit-hooks
    rev: v1.5.1
    hooks:
      - id: forbid-tabs
      - id: remove-tabs
        args: [--whitespaces-count, "4"]
  - repo: https://GitHub.com/pre-commit/mirrors-clang-format
    rev: v15.0.7
    hooks:
      - id: clang-format
        files: .*\.[hc]pp$
```

:::{exercise}
Ajoutez ce fichier dans votre dépôt et appliquez-le sur l'ensemble des fichiers à l'aide de la commande

```shell
pre-commit run --all-files
```
:::

À partir de maintenant, à chaque fois que vous ferez un commit, l'ensemble de ces règles seront vérifiées.

:::{note} Automatisation
Nous verrons dans la suite comment vérifier que ces règles sont vérifiées à chaque commit d'une pull request via GitHub Actions.
:::