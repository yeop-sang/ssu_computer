# -*- coding: utf-8 -*-

import pandas as pd
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

pd.options.display.max_columns=100
X.head()

from sklearn.model_selection import train_test_split
X_train,X_test,y_train,y_test=train_test_split(X,y,
                                               test_size=0.3,
                                               stratify=y,
                                               random_state=1)


# 앙상블 기반의 클래스를 로딩
# - 랜덤포레스트 : 배깅 앙승블에 결정트리를 조합한 모델이
#   주로 사용되어 하나의 클래스로 정의한 모델
from sklearn.ensemble import RandomForestClassifier


model = RandomForestClassifier(n_estimators=100,
                               max_depth=None,
                               max_samples=0.7,
                               max_features=0.7,
                               n_jobs=-1,
                               random_state=1)

model.fit(X_train, y_train)

# 평가
score = model.score(X_train, y_train)
print(f'Score (train) : {score} ')

score = model.score(X_test, y_test)
print(f'Score (test) : {score} ')
