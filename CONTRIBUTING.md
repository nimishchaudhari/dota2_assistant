# Contributing to Dota 2 AI Assistant

Thank you for your interest in contributing to the Dota 2 AI Assistant! This document provides guidelines and instructions for contributing to the project.

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read and understand the [Code of Conduct](CODE_OF_CONDUCT.md) before contributing.

## Development Workflow

1. **Pick an Issue**: Start by selecting an open issue from the GitHub issue tracker
2. **Comment on the Issue**: Let others know you're working on it
3. **Fork and Branch**: Fork the repository and create a feature branch
   ```
   git checkout -b feature/issue-XX-brief-description
   ```
4. **Develop and Test**: Make your changes with appropriate tests
5. **Follow Style Guidelines**: Ensure your code follows the project's style guide
6. **Commit with Care**: Write clear, concise commit messages
   ```
   git commit -m "Issue #XX: Brief description of changes"
   ```
7. **Pull Request**: Submit a pull request from your branch to the main repository

## Development Setup

Follow the setup instructions in the [README.md](README.md) to get your development environment ready.

## Testing

- Write unit tests for all new code
- Update existing tests when modifying code
- Run the test suite before submitting a PR:
  ```
  cd build
  ctest -C Release
  ```
- For tests that require Dota 2, clearly mark them with `[DOTA2]`
- Provide mock alternatives for CI/CD pipeline

## Pull Request Process

1. Ensure your code follows the style guidelines
2. Update documentation if necessary
3. Include tests for new functionality
4. Link the PR to the issue it resolves
5. Wait for a code review and address any feedback

## Style Guidelines

### C++ Code

- Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Use meaningful variable and function names
- Comment complex logic and public API
- Keep functions short and focused
- Use modern C++ features (C++17)

### Directory Structure

Maintain the established directory structure:
- Place new features in appropriate directories
- Create new directories only when necessary
- Follow existing naming conventions

## Documentation

- Update documentation for any feature changes
- Document public APIs with clear descriptions
- Include code examples where appropriate
- Keep the README.md up to date

## VAC Compliance

All contributions must adhere to Valve Anti-Cheat (VAC) policies:
- No direct game memory reading/writing
- No process injection or hooking
- Use only official GSI API for game data
- Create non-invasive overlays without game modification
- Do not expose information not normally available to players
- Do not automate player actions

## Questions?

If you have questions about contributing, please open an issue with the "question" label or contact the maintainers directly.

Thank you for helping improve the Dota 2 AI Assistant!
