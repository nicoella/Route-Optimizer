#!/bin/bash

# instructions
# ==================
# $ chmod +x setup.sh
# $ ./setup.sh
# ==================

npm install

touch src/config.json
echo '{
  "API_KEY": "[your api key here]"
}
' > src/config.json