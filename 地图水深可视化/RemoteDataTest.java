package com.yh.sipgwwms.project;

import com.yh.sipgwwms.project.bean.vo.remote.gps.GpsShipInfoVo;
import com.yh.sipgwwms.project.mapper.ShipMmsiInfoMapper;
import com.yh.sipgwwms.project.service.*;
import com.yh.sipgwwms.project.service.remote.ShipDataHandleService;
import com.yh.sipgwwms.project.service.remote.ShipMmsiInfoRemoteService;
import com.yh.sipgwwms.project.bean.vo.Point;
import com.yh.sipgwwms.project.utils.SpatialRelationUtil;
import net.sf.jsqlparser.expression.DateTimeLiteralExpression;
import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;

import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.*;


public class RemoteDataTest extends BaseTest {

    @Autowired
    private ShipMmsiInfoRemoteService shipMmsiInfoRemoteService;
    @Autowired
    private ShipMmsiInfoMapper shipMmsiInfoMapper;
    @Autowired
    private ShipService shipService;
    @Autowired
    private ShipRealtimeStateService shipRealtimeStateService;
    @Autowired
    private ShipReturnRecordService shipReturnRecordService;
    @Autowired
    private WorkAreaService workAreaService;
    @Autowired
    private DredgingAreaService dredgingAreaService;
    @Autowired
    private DumpingAreaService dumpingAreaService;
    @Autowired
    private RouteService routeService;
    @Autowired
    private QueueingTimeAbnormalService queueingTimeAbnormalService;
    @Autowired
    private NotEnteringDumpingAreaAbnormalService notEnteringDumpingAreaAbnormalService;
    @Autowired
    private SpeedAbnormalService speedAbnormalService;
    @Autowired
    private WrongRouteAbnormalService wrongRouteAbnormalService;
    @Autowired
    private PipelineService pipelineService;
    @Autowired
    private EarlyWarningService earlyWarningService;
    @Autowired
    private BerthingPointService berthingPointService;
    @Autowired
    private ShipDataHandleService shipDataHandleService;
    /**
     * 测试数据拉取
     */
    @Test
    public void addProjectTest() {
        try {
            List<Point> mPoints = new ArrayList<>();
            Point point1 = new Point();
            point1.setX(122.095698);
            point1.setY(31.103132);
            Point point2 = new Point();
            point2.setX(122.094416);
            point2.setY(31.101527);
            Point point3 = new Point();
            point3.setX(122.114866);
            point3.setY(31.090175);
            Point point4 = new Point();
            point4.setX(122.120148);
            point4.setY(31.091780);
            mPoints.add(point1);
            mPoints.add(point2);
            mPoints.add(point3);
            mPoints.add(point4);
            Point pointNow = new Point();
            pointNow.setX(122.101111);
            pointNow.setY(31.099048);
            boolean result = SpatialRelationUtil.isPolygonContainsPoint(mPoints, pointNow);
            System.out.println(result);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void TransportShipAbnormalCheckTest_sipgwwms_dev() { // 运泥船异常检测
        try {
            Calendar calendar= Calendar.getInstance();
            Long lo;
            List<GpsShipInfoVo> dataList = new ArrayList<>();
            //获取运泥船设备编号的map集合
            Map<String, String> decideMap = shipService.getAllShipDeviceNo(0);
            GpsShipInfoVo a=new GpsShipInfoVo();
            a.setGprs("101");

            int year=calendar.get(Calendar.YEAR);
            int month=calendar.get(Calendar.MONTH)-1;
            int today = calendar.get(Calendar.DAY_OF_MONTH);
            int hour=calendar.get(Calendar.HOUR_OF_DAY);
            int minute=calendar.get(Calendar.MINUTE);
            int second=calendar.get(Calendar.SECOND);

            calendar.set(year,month,today,hour-2,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(24.0);
            a.setLng(111.0);
            a.setSpeed(0.9);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(24.0);
            a.setLng(111.4);
            a.setSpeed(0.9);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 抛泥区排队超时异常 1
            dataList=new ArrayList<>();

            hour+=3;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(24.0);
            a.setLng(111.87);
            a.setSpeed(0.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 抛泥区排队超时异常 2
            dataList=new ArrayList<>();

            minute+=30;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(25.356);
            a.setLng(111.0);
            a.setSpeed(109.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 超速异常 1
            dataList=new ArrayList<>();

            minute+=40;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.0);
            a.setLng(115.0);
            a.setSpeed(129.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 超速异常 2
            dataList=new ArrayList<>();

            hour+=1;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(25.4);
            a.setLng(113.0);
            a.setSpeed(65.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 未按指定航线航行异常 1
            dataList=new ArrayList<>();

            hour+=1.5;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(25.123);
            a.setLng(113.9);
            a.setSpeed(61.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 未按指定航线航行异常 2
            dataList=new ArrayList<>();

            today++;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.0);
            a.setLng(116.5);
            a.setSpeed(74.852);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 当天未进入抛泥区异常 1
            dataList=new ArrayList<>();

            today++;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.0);
            a.setLng(116.9);
            a.setSpeed(60.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 当天未进入抛泥区异常 2
            dataList=new ArrayList<>();

            calendar.set(year,month,today,hour,minute+10,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.0);
            a.setLng(119.0);
            a.setSpeed(56.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // Enter 吹泥区
            dataList=new ArrayList<>();

            hour+=2;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.5);
            a.setLng(119.1);
            a.setSpeed(85.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 吹泥区排队超时异常 1
            dataList=new ArrayList<>();

            hour+=1.6;
            calendar.set(year,month,today,hour,minute+30,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(26.256);
            a.setLng(119.5);
            a.setSpeed(54.8745);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 吹泥区排队超时异常 2
            dataList=new ArrayList<>();

            System.out.println();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void TransportShipAbnormalCheckTest_sipgwwms_demo() { // 运泥船异常检测
        try {
            Calendar calendar= Calendar.getInstance();
            Long lo;
            List<GpsShipInfoVo> dataList = new ArrayList<>();
            //获取运泥船设备编号的map集合
            Map<String, String> decideMap = shipService.getAllShipDeviceNo(0);
            GpsShipInfoVo a=new GpsShipInfoVo();
            a.setGprs("14161371263"); // 设备编号

            int year=calendar.get(Calendar.YEAR);
            int month=calendar.get(Calendar.MONTH)-1;
            int today = calendar.get(Calendar.DAY_OF_MONTH);
            int hour=calendar.get(Calendar.HOUR_OF_DAY);
            int minute=calendar.get(Calendar.MINUTE);
            int second=calendar.get(Calendar.SECOND);

            calendar.set(year,month,today,hour-2,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.287413);
            a.setLng(121.597010);
            a.setSpeed(8.9);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.288697);
            a.setLng(121.603619);
            a.setSpeed(21.9);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 挖泥区排队超时异常 1
            dataList=new ArrayList<>();

            hour+=3;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.287706);
            a.setLng(121.609198);
            a.setSpeed(0.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 挖泥区排队超时异常 2
            dataList=new ArrayList<>();

            minute+=30;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.316564);
            a.setLng(121.671082);
            a.setSpeed(109.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 超速异常 1
            dataList=new ArrayList<>();

            minute+=40;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.310112);
            a.setLng(121.674344);
            a.setSpeed(79.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 超速异常 2
            dataList=new ArrayList<>();

            hour+=1;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(33.5);
            a.setLng(122.50);
            a.setSpeed(15.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 未按指定航线航行异常 1
            dataList=new ArrayList<>();

            hour+=1.5;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(25.123);
            a.setLng(113.9);
            a.setSpeed(61.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 未按指定航线航行异常 2
            dataList=new ArrayList<>();

            today++;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.0);
            a.setLng(116.5);
            a.setSpeed(74.852);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 当天未进入抛泥区异常 1
            dataList=new ArrayList<>();

            today++;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(27.0);
            a.setLng(118.9);
            a.setSpeed(60.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 当天未进入抛泥区异常 2
            dataList=new ArrayList<>();

            calendar.set(year,month,today,hour,minute+10,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(30.890906);
            a.setLng(121.713397);
            a.setSpeed(6.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // Enter 吹泥区
            dataList=new ArrayList<>();

            hour+=2;
            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(30.891274);
            a.setLng(121.716744);
            a.setSpeed(5.0);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 吹泥区排队超时异常 1
            dataList=new ArrayList<>();

            hour+=1.6;
            calendar.set(year,month,today,hour,minute+30,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(30.890758);
            a.setLng(121.718117);
            a.setSpeed(4.8745);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap); // 吹泥区排队超时异常 2
            dataList=new ArrayList<>();

            System.out.println();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void BerthintPointEarlyWarningCheckTest() {
        try {
            Calendar calendar= Calendar.getInstance();
            Long lo;
            List<GpsShipInfoVo> dataList = new ArrayList<>();
            //获取运泥船设备编号的map集合
            Map<String, String> decideMap = shipService.getAllShipDeviceNo(0);
            GpsShipInfoVo a=new GpsShipInfoVo();
            a.setGprs("101");

            int year=2022;
            int month=1;
            int today = 1;
            int hour=8;
            int minute=0;

            calendar.set(year,month,today,10,59,30);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(26.0);
            a.setLng(113.0);
            a.setSpeed(500.0);
            dataList.add(a);
            shipDataHandleService.BerthintPointEarlyWarningCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            calendar.set(year,month,today,10,59,30);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(20.5);
            a.setLng(114.5);
            a.setSpeed(500.0);
            dataList.add(a);
            shipDataHandleService.BerthintPointEarlyWarningCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            System.out.println();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void PipelineEarlyWarningCheckTest_sipgwwms() { // for 挖泥船
        try {
            Calendar calendar= Calendar.getInstance();
            Long lo;
            List<GpsShipInfoVo> dataList = new ArrayList<>();
            //获取运泥船设备编号的map集合
            Map<String, String> decideMap = shipService.getAllShipDeviceNo(0);
            GpsShipInfoVo a=new GpsShipInfoVo();
            a.setGprs("100"); // Swhua挖泥船

            int year=calendar.get(Calendar.YEAR);
            int month=calendar.get(Calendar.MONTH+1);
            int today = calendar.get(Calendar.DAY_OF_MONTH);
            int hour=calendar.get(Calendar.HOUR_OF_DAY);
            int minute=calendar.get(Calendar.MINUTE);
            int second=calendar.get(Calendar.SECOND);

            calendar.set(year,month,today,hour-2,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(24.0);
            a.setLng(111.0);
            a.setSpeed(10.566);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            calendar.set(year,month,today,10,59,30);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(24.0);
            a.setLng(111.4);
            a.setSpeed(20.1);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            System.out.println();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void PipelineEarlyWarningCheckTest_sipgwwms_demo() { // for 挖泥船
        try {
            Calendar calendar= Calendar.getInstance();
            Long lo;
            List<GpsShipInfoVo> dataList = new ArrayList<>();
            //获取运泥船设备编号的map集合
            Map<String, String> decideMap = shipService.getAllShipDeviceNo(0);
            GpsShipInfoVo a=new GpsShipInfoVo();
            a.setGprs("413794905"); // 挖泥船

            int year=calendar.get(Calendar.YEAR);
            int month=calendar.get(Calendar.MONTH+1);
            int today = calendar.get(Calendar.DAY_OF_MONTH);
            int hour=calendar.get(Calendar.HOUR_OF_DAY);
            int minute=calendar.get(Calendar.MINUTE);
            int second=calendar.get(Calendar.SECOND);

            calendar.set(year,month,today,hour-2,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.2871562);
            a.setLng(121.5963236);
            a.setSpeed(10.566);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            calendar.set(year,month,today,hour,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.2880363);
            a.setLng(121.5996711);
            a.setSpeed(20.1);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            calendar.set(year,month,today,hour+1,minute,second);
            lo=calendar.getTimeInMillis();
            a.setGpstime(lo);
            a.setLat(31.2884764);
            a.setLng(121.6026750);
            a.setSpeed(90.1);
            dataList.add(a);
            shipDataHandleService.AbnormalCheck(dataList, decideMap);
            dataList=new ArrayList<>();

            System.out.println();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    public void isPointInPolygonTest() {
        List<Point> mPoints=new ArrayList<>();
        Point point=new Point();
        point.setX(118.0);
        point.setY(26.0);
        mPoints.add(point);
        point.setX(120.0);
        point.setY(26.0);
        mPoints.add(point);
        point.setX(118.0);
        point.setY(28.0);
        mPoints.add(point);
        point.setX(120.0);
        point.setY(28.0);
        mPoints.add(point);

        point.setX(119.1);
        point.setY(27.5);

        System.out.println(SpatialRelationUtil.isPointInPolygon(mPoints, point));
    }
}
