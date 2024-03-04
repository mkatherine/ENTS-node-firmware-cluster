# Contributing Guidelines

Thank you for your interest in contributing to our project!

## Getting Started

Before you start, please make sure to have a look at our [Code of
Conduct](./CODE_OF_CONDUCT.md), which outlines our expectations for participants
within our community. We use the
[Contributor Covenant](https://www.contributor-covenant.org) with a copy
provided within the repository.

## How to Contribute

1. **Fork the repository** - You can do this by clicking the Fork button in the
top right of this page.

2. **Clone the repository** - This means downloading the repository to your
local machine. You can do this by clicking the green Code button and copying the
URL. Then, open your terminal and use the command `git clone [URL]`.

3. **Create a new branch** - You can do this using the command `git checkout -b
[branch-name]`.

4. **Make your changes** - Make the changes you want to contribute. These could
be adding new code, fixing a bug, or improving documentation.

5. **Commit your changes** - Use the command `git commit -m "[commit message]"`
to save your changes.

6. **Push your changes** - Use the command `git push origin [branch-name]` to
upload your changes to GitHub.

7. **Submit a pull request** - Go to your repository on GitHub, and click the
Compare & pull request button. If there are any issues that your PR resolves,
please link them in the description. Fill in the form and submit. The following
should be included in the PR description:
   - Your name, affiliation (aka current university or institution), and title.
     If you are working on the project for personal reasons state *no
     affiliation*. This is primarily to ensure there is no conflicts of interests
     for funding reasons.
   - The purpose of the PR. If the PR directly solves an issue it can be linked.
     Otherwise, we are interested in understanding how people are using the hardware
     or the reason being the feature being added.
   - The development environment. Provide the OS, hardware version, Platformio
     version, and information about any other tools used during the development.
   - Test procedure. For purely software, specifying the unit test is sufficient.
     Sometimes external configurations are required to test a hardware component.
     Create a new main in `examples/` and describe the expected output. A photo or
     diagram is very helpful.

8. **Code maintainer reviews the PR** - The final step is having a maintainer (see [README](./README.md)) review your code to check the code quality and test the functionality. If the code passes the review, the code will be merged. The maintainer will ask for any information you would like included in the contributors section of the [README](./README.md) in the comments of the PR. The maintainer will add the information in a future commit. Otherwise the maintainer will specify changes to the code. If a request is not clear, please ask for clarification. We will try to monitor the repository as much as possible. Ensure the code in the PR has the following:
   - Changes to [CHANGELOG.md](./CHANGELOG.md) describing your changes.
   - Code documentation. This is subjective, but we are expecting doxygen docstrings for any new additions to the code.

## Types of Contributions

### Reporting Bugs

If you find any bugs, please create a new issue in the issue tracker and tag them as bugs.

### 

## Code Style

Please adhere to the coding style used throughout the project. Consistency is
key!

In general we adhere to google style guides for both Python and C/C++. This may be difficult as code generated through CubeMX uses different and sometimes contradictory styles. If the file or `.c`/`.h` file pair you are working differs in style from the corresponding style guide, *use the style defined implicitly to maintain consistency within the file*.

- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Google Python Style Guide](https://google.github.io/styleguide/pyguide.html)

## Questions

If you have any questions, please don't hesitate to create an issue labeled
'question'.

Thank you again for your interest, and happy contributing!