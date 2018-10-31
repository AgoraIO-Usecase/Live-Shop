package io.agora.liveshop.activities;

import junit.framework.Assert;

import org.json.JSONException;
import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.agora.liveshop.data.Product;
import io.agora.liveshop.data.Quiz;
import io.agora.liveshop.data.QuizResult;

public class LiveRoomActivityTest {
    private LiveRoomActivity mActivity;
    private Product mProduct;
    private Quiz mQuiz;

    private static final String PRODUCT_JSON = "{\"type\": \"prod\", \"product\": {\"id\": \"awsxsad2121\"," +
            "\"title\": \"iphonex discount\",\"variants\": [{\"name\": \"Color\"," +
            "\"values\": [\"Black\",\"White\",\"Golden\"]}]," +
            "\"sold\": 20,\"price\": 400,\"currency\": \"$\",\"groupDeal\": 8000,\"streamId\": 2}}";

    private static final String QUIZ_JSON = "{\"type\": \"quiz\",\"question\": {\"id\": 2," +
            "\"content\": \"What's Jackie Chan\"," +
            "\"options\":[\"actor\",\"doctor\",\"teacher\", \"driver\"]," +
            "\"timeout\": 30,\"total\": 4}," +
            "\"result\": {\"result\": 0,\"total\": 3,\"spread\":{ \"0\": 25, \"1\": 7, \"2\": 35, \"3\": 92, \"-1\": 0 }}}";

    @Before
    public void init() {
        mActivity = new LiveRoomActivity();

        mProduct = new Product();
        mProduct.setId("awsxsad2121");
        mProduct.setTitle("iphonex discount");
        mProduct.setCurrency("$");
        mProduct.setOriginalPrice(400);
        mProduct.setSold(20);
        mProduct.setGroupPrice(8000);
        mProduct.setStreamId(2);

        mQuiz = new Quiz();
        mQuiz.setId(2);
        mQuiz.setTotal(4);
        mQuiz.setTimeout(30);
        mQuiz.setContent("What's Jackie Chan");

        QuizResult result = new QuizResult();
        result.setResult(0);
        result.setTotal(3);
        result.setOptionCount(4);
        result.setSpread(new int[] { 25, 7, 35, 92, 0 });
        mQuiz.setResult(result);
    }

    @After
    public void clean() {
        mActivity = null;
        mProduct = null;
        mQuiz = null;
    }

    @Test
    public void parseProductTest() {
        try {
            JSONObject object = new JSONObject(PRODUCT_JSON);
            Product product = mActivity.parseProduct(object);
            Assert.assertEquals(mProduct, product);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Test
    public void parseQuizTest() {
        try {
            JSONObject object = new JSONObject(QUIZ_JSON);
            Quiz quiz = mActivity.parseQuiz(object);
            Assert.assertEquals(mQuiz, quiz);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}