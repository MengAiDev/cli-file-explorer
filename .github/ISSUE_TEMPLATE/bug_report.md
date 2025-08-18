{
  "name": "Bug Report",
  "about": "Create a report to help us improve",
  "title": "[Bug]: ",
  "labels": ["bug"],
  "body": [
    {
      "type": "markdown",
      "attributes": {
        "value": "Thanks for taking the time to fill out this bug report!"
      }
    },
    {
      "type": "textarea",
      "id": "description",
      "attributes": {
        "label": "Describe the bug",
        "description": "A clear and concise description of what the bug is.",
        "placeholder": "Tell us what you see!"
      },
      "validation": {
        "required": true
      }
    },
    {
      "type": "textarea",
      "id": "reproduce",
      "attributes": {
        "label": "To Reproduce",
        "description": "Steps to reproduce the behavior:",
        "placeholder": "1. Go to '...'\n2. Click on '....'\n3. Scroll down to '....'\n4. See error"
      },
      "validation": {
        "required": true
      }
    },
    {
      "type": "textarea",
      "id": "expected",
      "attributes": {
        "label": "Expected behavior",
        "description": "A clear and concise description of what you expected to happen."
      },
      "validation": {
        "required": true
      }
    },
    {
      "type": "input",
      "id": "platform",
      "attributes": {
        "label": "Platform",
        "description": "What platform are you seeing the problem on?",
        "placeholder": "Linux, Windows, macOS"
      },
      "validation": {
        "required": true
      }
    },
    {
      "type": "input",
      "id": "version",
      "attributes": {
        "label": "Version",
        "description": "What version of our software are you running?"
      },
      "validation": {
        "required": true
      }
    },
    {
      "type": "textarea",
      "id": "additional",
      "attributes": {
        "label": "Additional context",
        "description": "Add any other context about the problem here."
      }
    }
  ]
}
