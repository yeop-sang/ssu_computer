"""
앙상블 (Ensemble)
    - 다수개의 모델을 결합하여 하나의 예측을 할 수 있는 결합 모델
    - 다수결의 원칙이 적용 (분류)
        - 일반화 성능(테스트 데이터에 대한 성능)을 극대화하기 위해서 생성된 모델이다.
    - 평균 원칙이 적용(회귀)


앙상블은 가장 좋은 평가 성적을 반환하지 않음
    - 앙상블은 하나의 모델 객체를 사용하지 않고
      다수개의 모델결과를 사용하여 다수결/평균을 취하므로
      앙상블을 구성하는 많은 모델에서 가장 좋은 성적의
      모델보다는 항상 평가가 떨어질 수 있음!
    - 일반화 성능을 극대화하는 모델임
    (테스트 데이터에 대한 성능)

앙상블을 구현하는 방법
    1. 취합모델(aggregation model)
        - 앙상블 구성하고 있는 각각의 모델이 **독립적**으로 동작
        - 각각의 모델이 독립적으로 학습하고 예측한 결과를 반환하여
          최종적으로 취합된 결과를 다수결 / 평균으로 예측함
        - Voting(다수결 방식)
        - Bagging
        - RandomForest
        - 병렬처리를 통해 학습과 예측의 속도를 매우 빠르게 올릴 수 있다.

    2. boosting
        - 취합이랑 반대
        - 앙상블을 구성하는 각각의 모델이 선형으로 결합되어
          점진적으로 학습의 성능을 향상시켜 나가는 방법
        - 부스팅의 첫번째 모델이 예측 결과 *
        - AdaBoosting, GradientBoosting(성능은 좋으나 속도가 느려서 사용되지 않음)
        - **"XGBoost"**(GradientBoosting를 병렬처리를 붙임!)
        - **"LightGBM"**(현업에서 제일 자주 쓰는 방식)
        - Boosting model 은 강한 제약 조건을 설정하여 점진적으로
          성능이 향상될 수 있도록 제어한다.
        - 모델이 독립적이지 않아 병렬처리가 불가능해서 학습시간이 매우 오래걸림

    시험 문제!
    - 10개의 모델의 취합 모델
        - 각 모델은 **과적합**시키는게 오히려 좋다!
        - 어차피 평균 낼 것이기 때문!
    - Boosting은 제약조건을 심하게 걸어서 학습시킨다.
"""

# base code
import pandas as pd
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                    test_size=0.3,
                                                    stratify=y,
                                                    random_state=1)

# ensemble 기반의 클래스를 로딩
from sklearn.ensemble import RandomForestClassifier

# 앙상블을 구성하는 각 모델의 클레스를 로딩
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
# 분류의 모델 수는 홀수로 하는 것이 유리함! 반반으로 나눠지면 성향을 찾기 힘듦!

# random_state는 항상 일정한 수를 넣어줘야지 성능 분석하기가 쉬움!
m1 = KNeighborsClassifier(n_jobs=-1)
m2 = LogisticRegression(random_state=1, n_jobs=-1, max_iter=5000)
m3 = DecisionTreeClassifier(max_depth=3, random_state=1)
estimators = [('knn',m1),('lr',m2),('dt',m3)]

"""
estimater : 예측 모델 
voting :{'hard' : 각 모델들은 모두 같은 가중치를 가지고 있음, 'soft' : 가중치를 넣어줌}
weights : 가중치 리스트
n_jobs : 병렬처리 한계치 -1은 무제한
"""
model = RandomForestClassifier(
    na
                         )

model.fit(X_train, y_train)

score = model.score(X_train, y_train)

t_score = model.score(X_test, y_test)

pred = model.predict(X_test[:1])

pred = model.estimators_[0].predict(X_test[:1])



























