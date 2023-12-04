# Documentation

Nous allons à présent générer la documentation de notre logiciel et la rendre disponible via [GitHub Pages](https://pages.github.com/). Nous utiliserons [Sphinx](https://www.sphinx-doc.org), [breathe](https://breathe.readthedocs.io) et [doxygen](https://www.doxygen.nl/) pour générer la documentation en html. La configuration comporte un certain nombre d'étapes et nous ne souhaitons pas perdre de temps sur ce point. Nous avons néanmoins mis une partie à la fin de ce chapitre qui décrit les étapes si vous voulez le refaire plus tard.

:::{note}
N'oubliez pas de retourner dans la branche `main` et de faire

```bash
git pull
```
:::

Nous avons donc mis dans le répertoire `4.documentation/required_files` un répertoire `doc` qui contient la configuration et les fichiers markdown constituant la documentation. Il vous suffit de le copier dans votre répertoire.

Créez une nouvelle branche.

## Test local

Nous allons essayer de générer la documentation en local pour s'assurer que tout se passe correctement. Pour cela, nous avons ajouté une tâche dans le fichier `pixi.toml` qui fait la chose suivante

```toml
doc = { cmd = "cd doc; make html" }
```

Exécutez cette tâche et vérifiez que tout se passe bien.

:::{tip} html
Vous devriez avoir un dossier `html` qui s'est créé. Il vous suffit d'ouvrir dans votre navigateur le fichier `index.html`.
:::

## Mise en place de l'action

Nous allons ajouter une action pour la documentation dans le dossier `.github/workflows` que nous appelons `doc.yml`.

Écrivez l'action en vous appuyant sur ce que vous avez fait précédemment. Pour le moment, nous souhaitons juste faire ce que nous avons fait en local et mettre le répertoire `html` dans une archive : voir l'action [upload-pages-artifact](https://github.com/actions/upload-pages-artifact).

:::{tip} Artifact
A la foin de cette étape, vous devriez avoir accès à une archive en allant dans les actions de votre dépôt. Si vous téléchargez cette archive et que vous regardez ce qu'il y a dedans, vous devriez retrouver le contenu du répertoire `html`.
:::

## Configuration de GitHub Pages

Vous devez configurer GitHub Pages en allant dans les `Settings` de votre dépôt puis dans `Pages` et sélectionner pour les `Source` `GitHub Actions` comme sur cette image

![](./figures/pages.png)

Vous devez également ajouter dans votre fichier `doc.yml`, les lignes suivantes

```yaml
# Allow one concurrent deployment
concurrency:
  group: "pages"
  cancel-in-progress: true
```

Ceci afin d'éviter que plusieurs actions essaient de générer le site en même temps.

:::{note} Pour plus d'information
[Using concurrency](https://docs.github.com/en/actions/using-jobs/using-concurrency)
:::

## Mise en place de la documentation

Nous expliquons ici comment initialiser le répertoire documentation en utilisant les outils cités au début de ce chapitre : Sphinx, Breathe et Doxygen. Les deux derniers sont utilisés pour pouvoir mettre la documentation C++ des fonctions, classes... dans Sphinx.

Il nous faut d'abord créer l'architecture du répertoire Sphinx en utilisant la ligne de commande

```bash
sphinx-quickstart doc
```

Vous allez devoir répondre à un certain nombre de questions. Toutes vos réponses se trouveront dans un fichier `conf.py`. Vous pourrez donc revenir sur vos choix à tout moment.

Nous allons à présent configurer Doxygen. Vous devez aller dans le répertoire `doc` et créer un fichier de configuration `doxyfile` reconnue par celui-ci. Il suffit d'exécuter la ligne de commande

```bash
doxygen -g doxyfile
```

Il nous reste à modifier quelques entrées de ce fichier de la manière suivante

```txt
PROJECT_NAME = "splinart"
XML_OUTPUT = xml
INPUT = ../include
GENERATE_XML = YES
```

À partir de cette étape, si vous exécutez la ligne de commande

```bash
doxygen
```

vous devriez générer la documentation au format XML utilisé par Breathe.

Nous allons à présent ajouter les extensions nécessaires dans la configuration de Sphinx. À savoir

```python
extensions = ['breathe', 'myst_parser']
breathe_projects = { 'splinart': '../xml' }
```

:::{note}
Nous avons aussi ajouté l'extension [myst-parser](https://myst-parser.readthedocs.io) qui permet d'avoir un langage markdown enrichi. Si vous voulez des extensions particulières, il faut les ajouter dans la configuration de Sphinx. Nous avons ajouté la liste suivante

```python
myst_enable_extensions = [
    "amsmath",
    "attrs_inline",
    "colon_fence",
    "deflist",
    "dollarmath",
    "fieldlist",
    "html_admonition",
    "html_image",
    "replacements",
    "smartquotes",
    "strikethrough",
    "substitution",
    "tasklist",
]
```
:::

Nous allons maintenant changer le thème par défaut en mettant [pydata-sphinx-theme](https://pydata-sphinx-theme.readthedocs.io) dnas le fichier de configuration

```python
html_theme = 'pydata_sphinx_theme'
```

Enfin, il faut modifier le fichier `Makefile` qui se trouve dans le répertoire `doc` pour que Doxygen soit déclenché à chaque fois que vous lancez la génération de la documentation. Voici le `Makefile` complet

```makefile
# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	doxygen
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
```

:::{attention}
Si vous êtes sous Windows, c'est le fichier `make.bat` qu'il faut modifier.
:::

Il ne vous reste plus qu'à écrire la documentation. Nous vous laissons regarder dans le répertoire `material/4.documentation/required_files/doc/source` pour voir un exemple. Das le répertoire `api`, vous verrez l'utilisation de Breathe.
