#!/bin/bash

# instructions
# ==================
# $ chmod +x setup.sh
# $ ./setup.sh
# ==================

npm install

touch config.json
echo '{
  "API_KEY": "[your api key here]"
}
' > config.json