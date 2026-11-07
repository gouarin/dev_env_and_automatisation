# Création de version et packaging

:::{hint}Objectifs de cette partie
- Utiliser les commits conventionnels
- Comprendre les montées de version avec semver
- Construire un paquet conda
:::

:::{note}Notions GitHub Actions abordées
- Vérifier le titre d'une pull request
- Construire automatiquement une nouvelle version
- Jouer avec les variables d'environnement
- Utiliser des mots de passe ou token
:::

Nous voici dans la dernière partie de cet atelier. Nous souhaitons à présent pouvoir déclencher un événement à la main qui

- crée un `CHANGELOG` à partir des commits réalisés depuis la dernière version,
- met à jour la version,
- crée une archive de cette nouvelle version,
- lance la construction d'un paquet `conda` et dépose celui-ci dans votre `channel`.

:::{note}
N'oubliez pas de retourner dans la branche `main` et de faire

```bash
git pull
```
:::

Jusqu'à présent, nous n'avons pas fait attention si le nom de la pull request satisfaisait aux critères du [commit conventionnel](#conventional-commits). Hors, cela devient ici indispensable pour que le `CHANGELOG` soit généré correctement et automatiquement. Nous allons donc commencer par mettre une action en place qui vérifie que le titre de la pull request suit la nomenclature du commit conventionnel.

## Première version

Afin d'automatiser le processus de création de version de manière automatique, nous devons l'initier. Pour cela, nous allons créer un fichier `version.txt` à la racine du projet avec le contenu suivant

```text
0.0.1
```

Une fois que cette modification est fusionnée dans la branche principale, il suffit de créer une première version du projet avec le même numéro de version.

Allez sur votre dépôt GitHub et cliquez sur `Release` qui se trouve dans la colonne de droite. Puis `Create a new release`.

Allez dans `Choose a tag` et rentrez `v0.0.1`. Et enfin `Publish release`.

En retournant sur la page d'accueil de votre dépôt, vous devriez voir apparaître en dessous de `Release` votre `v0.0.1`.

## Vérification du titre de la pull request

Nous allons à présent vérifier le titre de la pull request. Tout se passe encore une fois dans la section `on` qui gère les événements. Nous allons à présent utiliser l'événement [`pull_request_target`](https://docs.github.com/actions/using-workflows/events-that-trigger-workflows#pull_request_target) qui intervient à chaque fois qu'une opération est demandée sur l'interface de la pull request.

Nous allons sélectionner trois types d'activités

- `opened`: lorsque la pull request est créée
- `edited`: lorsque le titre de la pull request est modifiée
- `synchronize`

Nous utiliserons l'action [semantic-pull-request](https://github.com/marketplace/actions/semantic-pull-request) pour la validation.

Créez l'action associée, testez et fusionnez dans la branche principale.

## Création automatique d'une nouvelle version

À présent, à partir des titres de la pull request, nous allons pouvoir déterminer les versions successives en utilisant la gestion sémantique de version [semver](https://semver.org). Nous n'allons pas rentrer dans les détails, mais donner juste assez d'éléments pour la compréhension de la suite. Un numéro de version peut être décomposé comme suit

:::{figure} ./figures/semver.png
:::

La version corrective correspond à une correction qui corrige un comportement interne incorrect, mais qui ne casse pas l'API actuelle. La version mineure correspond à l'ajout de nouvelles fonctionnalités qui ne cassent pas l'API actuelle. Enfin, la version majeure correspond à des changements qui cassent l'API actuelle.

Généralement, lorsque la version majeure est supérieure ou égale à `1`, le logiciel sort d'un état de développement pour aller vers un état dit stable. Notre projet est un problème jouet et ne peut être considéré comme un logiciel dans un état stable. Nous garderons donc la version majeure à `0`. Par conséquent, si la version mineure est augmentée, cela signifie que des nouvelles fonctionnalités ont été ajoutées et que celles-ci peuvent casser ou non l'API actuelle.

Revenons aux commits conventionnels et à leur influence sur la montée de version.

- Si depuis la dernière version, l'ensemble des commits sont du type `fix`, alors seule la version corrective sera incrémentée.
- Si depuis la dernière version, au moins un des commits est de type `feat`, alors la version mineure est incrémentée et la version corrective est remise à zéro.

Comment générer ce modèle de manière automatique sur notre dépôt ? En utilisant GitHub Actions !

Nous allons utiliser cette action : [release-please-action](https://github.com/marketplace/actions/release-please-action) qui utilise [release-please](https://github.com/googleapis/release-please).

`release-please` a besoin de fichiers de configuration pour contrôler son comportement. Voici les deux fichiers à ajouter à la racine du dépôt : vous les trouverez dans le répertoire `material/6.packaging/required_files`.

```{literalinclude} ../material/6.packaging/required_files/release-please-config.json
:language: json
```

```{literalinclude} ../material/6.packaging/required_files/.release-please-manifest.json
:language: json
```

La clé `packages` indique que le projet à publier se trouve à la racine du dépôt (`.`). Ne l'oubliez pas : sans elle, release-please ne trouve rien à publier et se termine sans afficher d'erreur.

:::{tip} `release-please-config.json`

Si vous voulez comprendre ce que fait ce fichier de configuration, il vous suffit de vous rendre à [Manifest Driven release-please](https://github.com/googleapis/release-please/blob/main/docs/manifest-releaser.md).

:::

:::{tip} Type de déclenchement

Nous déclencherons l'action à la main. Par conséquent

```yaml
on: workflow_dispatch
```
:::

Cette action se joue en deux temps.
- La première fois que vous la déclenchez, elle ouvre une pull request avec la version qui sera créée et le `CHANGELOG.md`.
- La deuxième fois que vous la déclenchez, elle crée la nouvelle version dans les `Release` de votre dépôt GitHub.

::::{important}Utilisation d'un token

Il est nécessaire de mettre un `token` pour qu'à la fin de la deuxième étape, une action de type `release` soit déclenchée. Les événements créés avec le token fourni par défaut à GitHub Actions (`GITHUB_TOKEN`) ne déclenchent pas d'autres workflows : GitHub évite ainsi les déclenchements en cascade. Il vous faut donc un [personal access token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens) de type *fine-grained*, limité à votre dépôt.

Allez dans la configuration de votre compte `Settings->Developer settings->Personal access tokens->Fine-grained tokens` et cliquez sur `Generate new token`. Puis

- dans `Repository access`, choisissez `Only select repositories` et sélectionnez votre dépôt `splinart-cpp` ;
- dans `Permissions`, donnez l'accès `Read and write` à `Contents` (pour créer les tags et les releases) et à `Pull requests` (pour ouvrir la pull request de version) ;
- choisissez une date d'expiration : il faudra renouveler le token ensuite.

Ajoutez ce token dans les `secrets` du dépôt en allant dans `Settings->Secrets and variables->Actions`. Vous l'appellerez `RELEASE_PLEASE`.

::::

::::{important}Droits pour GitHub Actions

La création de la pull request nécessite que GitHub Actions ait des droits sur votre dépôt. Il faut aller dans `Settings->Actions->General` et cocher la case tout en bas

:::{figure} ./figures/action-perm.png
:::
::::

Créez l'action associée et ajoutez les fichiers de configuration à votre dépôt. Vous pouvez fusionner à la branche principale.

D'ici, vous devriez avoir deux commits depuis la dernière version et ceux-ci devraient suivre la nomenclature des commits conventionnels. Vous pouvez donc tester la création d'une nouvelle version !

Vous pouvez également tester en faisant des pull request qui mixent des `fix` et des `feat` et voir le comportement.

## Packaging conda

Nous allons montrer dans cette section comment déclencher la construction d'un paquet conda lorsqu'une nouvelle version est disponible, et comment le rendre directement accessible sur votre `channel` conda.

:::{attention}
Nous faisons cela à titre d'exemple. Si vous voulez créer un paquet conda et le distribuer, il est préférable de le mettre sur [conda-forge](https://conda-forge.org/docs/maintainer/adding_pkgs.html) pour plus de visibilité.
:::

### Construction locale

La construction d'un paquet conda se fait à partir d'une recette. Copiez le répertoire `material/6.packaging/required_files/recipe` à la racine de votre dépôt. Il contient

- un fichier `recipe.yaml` qui décrit le paquet : son nom, sa version, où trouver les sources, les dépendances nécessaires à la construction et à l'exécution, et un test ;
- un fichier `build.sh` qui décrit comment construire le paquet sous Linux et macOS ;
- un fichier `build.bat` qui décrit comment construire le paquet sous Windows.

```{literalinclude} ../material/6.packaging/required_files/recipe/recipe.yaml
:language: yaml
```

Quelques remarques sur cette recette

- La version est lue dans la variable d'environnement `VERSION`. Nous la remplirons à partir du fichier `version.txt`.
- `source: path: ..` indique que les sources sont celles de votre dépôt, juste au-dessus du répertoire `recipe`. Les fichiers ignorés par git, comme le répertoire `.pixi`, ne sont pas copiés.
- `${{ compiler("cxx") }}` choisit le compilateur C++ adapté à chaque OS.
- Le test vérifie simplement que l'exécutable installé répond à l'option `--help`.

Nous construisons le paquet avec [rattler-build](https://rattler.build). Nous ne l'ajoutons pas à l'environnement du projet : rattler-build y amènerait des versions plus récentes de certaines bibliothèques (comme `libcxx` sous macOS) qui ne fonctionnent plus avec le compilateur du projet. La commande `pixi exec` lance un outil dans un environnement temporaire, séparé de celui du projet.

Lancez la construction

::::{tab-set}
:::{tab-item}Linux & macOS
```bash
VERSION=$(cat version.txt) pixi exec rattler-build build --recipe recipe --output-dir output
```
:::
:::{tab-item}Windows
```powershell
$env:VERSION = Get-Content version.txt
pixi exec rattler-build build --recipe recipe --output-dir output
```
:::
::::

Si tout s'est bien passé, le paquet se trouve dans le répertoire `output`, dans un sous-répertoire qui porte le nom de votre plateforme (par exemple `output/linux-64/splinart-cpp-0.0.1-h....conda`). Ajoutez `output` à votre fichier `.gitignore`.

:::{note}
Si vous reconstruisez la même version après avoir modifié la recette, supprimez d'abord le répertoire `output`. Sinon, l'index du répertoire peut garder l'empreinte de l'ancien paquet et pixi refusera de l'installer.
:::

Nous allons essayer d'installer ce paquet avec pixi. Le répertoire `output` est un `channel` conda local. Créez un projet de test en dehors de votre dépôt, ajoutez-lui ce `channel` en premier, puis installez le paquet

```bash
pixi init test-splinart
cd test-splinart
pixi workspace channel add --prepend file:///chemin/vers/splinart-cpp/output
pixi add splinart-cpp
pixi run splinart-cpp
```

Une fenêtre doit s'ouvrir avec un cercle.

### Anaconda

Nous allons maintenant mettre le paquet construit localement sur [anaconda.org](https://anaconda.org/) pour être sûr que tout le processus fonctionne avant de l'automatiser. Vous devez donc vous créer un compte : son nom sera celui de votre `channel`.

Pour déposer un paquet, que ce soit depuis votre machine ou depuis une action, il faut s'authentifier avec un token. Une fois le compte créé, cliquez sur votre avatar et choisissez `Settings`. Allez ensuite dans `Access` et créez un token qui a les droits suivants

:::{figure} ./figures/anaconda.png
:::

:::{note}
Le nom que vous donnez au token n'a pas de réelle importance.
:::

Vous pouvez maintenant déposer le paquet sur votre compte

::::{tab-set}
:::{tab-item}Linux & macOS
```bash
ANACONDA_API_KEY=votre_token pixi exec rattler-build upload anaconda --owner votre_compte output/*/splinart-cpp-*.conda
```
:::
:::{tab-item}Windows
```powershell
$env:ANACONDA_API_KEY = "votre_token"
pixi exec rattler-build upload anaconda --owner votre_compte output\win-64\splinart-cpp-0.0.1-xxx_0.conda
```

Remplacez le nom du fichier par celui que rattler-build a produit.
:::
::::

Si tout s'est bien passé, vous devriez voir votre paquet sur votre compte anaconda. Pour tester l'installation, retournez dans le projet `test-splinart`, remplacez le `channel` local par le vôtre et réinstallez le paquet

```bash
pixi workspace channel remove file:///chemin/vers/splinart-cpp/output
pixi workspace channel add --prepend votre_compte
pixi add splinart-cpp
```

### Construction et déploiement du paquet conda

Maintenant que nous avons vérifié que tout se passe bien localement, nous allons ajouter une action qui construit le paquet sur Linux, macOS et Windows, et qui le dépose sur votre compte anaconda lorsqu'une nouvelle version est publiée.

Il y a ici deux ingrédients

- Encore une fois, nous allons jouer avec la section `on` pour déclencher l'événement et plus précisément avec [`release`](https://docs.github.com/actions/using-workflows/events-that-trigger-workflows#release).

  ```yaml
  on:
    release:
      types:
        - published
  ```

- Il nous faut également donner le token à l'action en ajoutant un `secret` dans le dépôt GitHub.

  :::{attention}
  Il ne faut jamais mettre de mots de passe ou de tokens en clair dans vos fichiers !
  :::

  Pour ajouter le token dans les `secrets` de GitHub, allez dans les `Settings` de votre dépôt puis dans `Secrets and variables -> Actions`. Cliquez ensuite sur `New repository secret`. Donnez-lui un nom et copiez-collez le token créé sur votre compte anaconda.

  :::{attention}
  Le nom du `secret` sera celui utilisé dans l'action.
  :::

Voici à quoi ressemble l'action. Elle se trouve aussi dans le répertoire `material/6.packaging/required_files/.github/workflows`.

```{literalinclude} ../material/6.packaging/required_files/.github/workflows/conda.yml
:language: yaml
:caption: .github/workflows/conda.yml
```

Plusieurs remarques

- Nous construisons le paquet à chaque pull request pour être sûr de ne pas avoir cassé sa construction.
- Nous pouvons aussi lancer la construction à la main (`workflow_dispatch`).
- La matrice couvre Linux, macOS Intel (`macos-15-intel`), macOS Apple Silicon (`macos-latest`) et Windows.
- `run-install: false` installe pixi sans installer l'environnement du projet, dont la construction n'a pas besoin.
- L'étape `Read the version` écrit `VERSION` dans le fichier `$GITHUB_ENV` : les étapes suivantes du travail voient cette variable d'environnement. Elle utilise `shell: bash`, disponible sur tous les OS, y compris Windows.
- La construction tourne dans le shell par défaut de chaque OS (PowerShell sous Windows). rattler-build choisit lui-même `build.sh` ou `build.bat`.
- Nous ne déposons le paquet sur anaconda.org que si nous sommes dans un événement `release`
  ```yaml
  - name: Upload on anaconda.org
    if: github.event_name == 'release'
  ```
- Nous nous servons du `secret` ajouté précédemment, transmis à rattler-build par la variable d'environnement `ANACONDA_API_KEY`.

  :::{note}
  Nous avons choisi ici `ANACONDA_TOKEN` comme nom de `secret`. Pensez à le remplacer par le nom que vous aurez choisi, et `votre_compte` par le nom de votre compte anaconda.
  :::

Si la pull request arrive à construire le paquet conda, vous pouvez fusionner cette branche dans la branche principale.

Testez cette action en créant au préalable une nouvelle version : c'est-à-dire en exécutant deux fois l'action `release-please` (une fois pour créer une pull request avec le changelog et la montée de version et une deuxième fois pour créer le tag et l'archive).

:::{important}
La correction de cette partie, et de tout l'atelier, se trouve dans le dépôt [splinart-cpp](https://github.com/gouarin/splinart-cpp). Vous y trouverez notamment les workflows qui vérifient le titre des pull requests (`check-pr-title.yml`), créent les versions (`release-please.yml`) et publient le paquet conda (`publish.yml`).
:::
