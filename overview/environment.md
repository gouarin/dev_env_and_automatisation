# Environnement de développement

:::{hint}Objectifs de cette partie
- Définir ce qu'est un environnement de développement
- Présenter les différents modèles
- Comprendre les enjeux associés
- Utiliser pixi pour illustrer la mise en place d'un environnement de développement
:::

## Présentation générale

La mise en place d'un cadre de développement passe par la définition des outils et des dépendances nécessaires pour développer et tester son application ou sa librairie. Le but est que toute personne désireuse de s'investir dans le projet puisse le faire rapidement et simplement. De plus, si un problème se présente lors de l'installation ou de la phase de test, les itérations pour résoudre le problème seront réduites, car l'environnement de développement est figé. Il y a donc de forte chance que le problème vienne de notre application et non de la mise à jour d'une dépendance.

Nous retrouvons dans de nombreux langages des outils de gestion de paquets plus ou moins aboutis. La spécification de l'environnement peut se faire via un fichier écrit entièrement par une personne. C'est le cas par exemple du fichier `requirements.txt` que nous retrouvons dans le langage Python permettant de décrire un ensemble de paquets. Le problème avec cette démarche est qu'il est facile d'oublier une dépendance et qu'il est difficile de définir clairement les versions souhaitées.

D'autres systèmes de gestion de paquets ont fait un réel effort pour construire des environnements qui se veulent reproductibles. Nous pouvons par exemple citer [yarn](https://yarnpkg.com) pour les projets JavaScript ou encore [cargo](https://doc.rust-lang.org/cargo/) pour les projets Rust. Pour chacun d'entre eux, un fichier local au projet indique les paquets installés. Lorsqu'une nouvelle dépendance est installée, celle-ci est directement ajoutée à ce fichier en spécifiant une version. Il existe également un autre fichier (généralement avec l'extension `.lock`) permettant de réinstaller à l'identique pour l'OS et l'architecture concernés.

:::{note}
[poetry](https://python-poetry.org/) permet d'avoir le même cas d'usage que yarn et cargo pour le langage Python.
:::

:::{attention}
La reproductibilité n'est pas facile à obtenir et selon la version et le moment où vous utilisez le logiciel qui fait la gestion de packages, rien ne vous dit que votre environnement de développement sera réellement reproductible dans un avenir plus ou moins proche.
:::

Il existe aussi des systèmes de gestions de paquets qui ne sont pas spécifiques à un langage. Nous pouvons citer [Nix](https://nixos.org/), [Guix](https://guix.gnu.org/) ou encore [Spack](https://GitHub.com/spack/spack). La particularité de Nix et de Guix est d'imposer la reproductibilité par design. Vous êtes assurés d'avoir le même environnement de développement au bit près. Chacune de ces méthodes recompile l'intégralité de l'arbre des dépendances sur votre machine. Vous pouvez de plus avoir un dépôt distant où les binaires sont disponibles.

Si ces systèmes sont intéressants pour un usage en calcul scientifique et notamment dans la mise en place d'environnements de développement et de production sur des machines HPC, leur courbe d'apprentissage peut être assez raide.

Nous pouvons par ailleurs citer l'univers [conda](https://docs.conda.io) et notamment [conda-forge](https://conda-forge.org/) qui rassemble un ensemble de paquets créés et diffusés par la communauté. Ces paquets n'ont pas de langages de programmation particuliers. Nous retrouvons des paquets écrits en C++, Python, R, ... En revanche, comme dit plus haut pour le langage Python, la mise en place d'un environnement de développement passe par l'écriture d'un fichier `environment.yml` qui est source d'erreurs humaines. Pour palier à ce problème et avoir les mêmes avantages que yarn ou cargo, [pixi](https://pixi.sh/) a vu le jour cette année.

## pixi

pixi va être utilisé dans la suite pour mettre en place l'environnement de développement qui va nous servir tout au long de cet atelier. C'est pourquoi, nous allons détailler ici son utilisation.

### Installation

Pour installer pixi, il suffit de taper cette ligne de commande dans un terminal

::::{tab-set}
:::{tab-item}Linux & macOS
```bash
curl -fsSL https://pixi.sh/install.sh | bash
```
:::
:::{tab-item}Windows
```powershell
powershell iwr -useb https://pixi.sh/install.ps1 | iex
```
:::
::::

:::{note}
Pour vérifier que tout s'est bien passé, vous pouvez essayer d'afficher l'aide

```bash
pixi --help
```

:::

### Création de l'environnement

Nous allons voir à présent comment a été créé l'environnement afin de bien comprendre le fonctionnement. Nous verrons ensuite comment le recréer sans passer par toutes ces étapes.

Nous allons donc initialiser un nouveau projet en faisant dans un terminal

```bash
pixi init test-pixi
cd test-pixi
```

Notre projet a plusieurs dépendances et nous allons les installer au fur et à mesure. L'application est écrite en C++ et nous choisirons [CMake](https://cmake.org/) pour compiler et installer celle-ci. Pour installer ces dépendances, il suffit d'utiliser la commande `add` de pxi.

```bash
pixi add cxx-compiler cmake
```

À la fin de cette étape, nous avons deux nouveaux fichiers : `pixi.toml` et `pixi.lock`. Le contenu de `pixi.toml` est le suivant

```toml
[project]
name = "test-pixi"
version = "0.1.0"
description = "Add a short description here"
authors = ["Loic Gouarin <loic.gouarin@gmail.com>"]
channels = ["conda-forge"]
platforms = ["osx-arm64"]

[tasks]

[dependencies]
cmake = "3.27.8.*"
cxx-compiler = "1.6.0.*"
```

:::{attention}
Vous n'avez pas forcément la même plateforme. Les plateformes possibles sont : `win-64`, `linux-64`, `osx-64`, `osx-arm64`. Vous pouvez les ajouter au fur et à mesure. Il est également possible d'avoir un comportement différent en fonction de celle-ci ([Multi platform](https://pixi.sh/advanced/multi_platform_configuration/)).
:::

Comme dans le cas de nodejs avec le répertoire `node_modules`, vous avez à présent un répertoire `.pixi` dans lequel vous avez votre environnement conda.

Pour utiliser cet environnement, vous avez deux possibilités: la commande `run` ou la commande `shell`.

La commande `run` est suivie de la commande que vous voulez exécuter dans l'environnement. Par exemple

```bash
pixi run cmake --version
```

alors que la commande `shell` vous place directement dans un shell lié à l'environnement dans lequel vous pouvez exécuter un ensemble de commandes. Par exemple

```bash
pixi shell
cmake --version
exit
```

### Mise en place de tâches

L'autre avantage de pixi est la possibilité de mettre en place des tâches et de les appeler via pixi. Prenons l'exemple de la configuration du projet via CMake. Il suffit de rajouter une entrée `[tasks]` dans le fichier `pixi.toml`

```toml
# Configures CMake
configure = { cmd = [
    "cmake",
    # Use the cross-platform Ninja generator
    "-G",
    "Ninja",
    # The source is in the root directory
    "-S",
    ".",
    # We wanna build in the .build directory
    "-B",
    ".build",
] }
```

:::{attention}
Il ne faut pas oublier d'installer `ninja` pour que cette tâche fonctionne.`
:::

Pour exécuter cette tâche, il suffit de taper la ligne de commande suivante dans un terminal

```bash
pixi run configure
```

Nous allons par la suite utiliser ces tâches pour décrire comment compiler le projet, comment réaliser les tests, la documentation, ...

## Conclusion

Le cadre mis en place ici permet à toute nouvelle personne souhaitant contribuer au projet de facilement être autonome et opérationnel grâce à l'installation d'un environnement de développement sur sa propre machine. La description des tâches possibles permet d'exécuter exactement le même processus que celui que vous avez mis en place et que vous utilisez à chaque changement. Le gain de temps est énorme et ce processus bien structuré et reproductible permet d'éviter le découragement des bonnes volontés.

Nous avons donné quelques exemples d'outils affiliés ou non à des langages de programmation. N'hésitez pas à faire des recherches pour votre langage préféré. Vous devriez trouver votre bonheur.