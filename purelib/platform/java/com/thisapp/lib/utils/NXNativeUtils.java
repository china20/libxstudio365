package com.thisapp.lib.utils;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.provider.Settings;

import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Enumeration;
import java.util.UUID;

import android.content.Context;
//import android.location.Criteria;
//import android.location.Location;
//import android.telephony.TelephonyManager;
//import android.util.Log;
import android.content.pm.PackageManager;

public class NXNativeUtils {

	/** 没有网络 */
	public static final int NETWORKTYPE_INVALID = 0;
	/** wap网络 */
	public static final int NETWORKTYPE_WAP = 1;
	/** 2G网络 */
	public static final int NETWORKTYPE_2G = 2;
	/** 3G和3G以上网络，或统称为快速网络 */
	public static final int NETWORKTYPE_3G = 3;
	/** wifi网络 */
	public static final int NETWORKTYPE_WIFI = 4;

	static class PermissionMissingException extends Exception {
	}

	private static Activity thisApp = null;
	private static Context appCtx = null;

	private static TelephonyManager sTelephonyManager = null;
	private static WifiManager sWifiManager = null;
	private static boolean sbSimCardReady = false;

	public static void init(Activity ctx) {
		thisApp = ctx;
		appCtx = thisApp.getApplicationContext();
		sTelephonyManager = (TelephonyManager) thisApp
				.getSystemService(Context.TELEPHONY_SERVICE);
		sbSimCardReady = (TelephonyManager.SIM_STATE_READY == sTelephonyManager
				.getSimState());
		sWifiManager = (WifiManager) thisApp
				.getSystemService(Context.WIFI_SERVICE);
	}

	public static String getCompatibleDeviceId()
			throws PermissionMissingException {
		final String tmDevice, tmSerial, androidId;

		final TelephonyManager tm = (TelephonyManager) appCtx
				.getSystemService(Context.TELEPHONY_SERVICE);

		if (checkPermission(appCtx, Manifest.permission.READ_PHONE_STATE)) {
			tmDevice = "" + tm.getDeviceId();
			tmSerial = "201505291355"; // + tm.getSimSerialNumber();
		} else { // TODO: throw exception, AndoridMenifest.xml missing
					// permission: READ_PHONE_STATE
			throw new PermissionMissingException();
		}
		androidId = ""
				+ Settings.Secure.getString(appCtx.getContentResolver(),
						Settings.Secure.ANDROID_ID);

		UUID deviceUuid = new UUID(androidId.hashCode(),
				((long) tmDevice.hashCode() << 32) | tmSerial.hashCode());
		System.out.println("UUID=" + deviceUuid.toString());
		return deviceUuid.toString();
	}

	public static String getIMEI() {
		return sTelephonyManager.getDeviceId();
	}

	public static String getIMEISV() {
		return sTelephonyManager.getDeviceSoftwareVersion();
	}

	public static String getIMSI() {
		if (sbSimCardReady) {
			return sTelephonyManager.getSubscriberId();
		}
		return "Your SIM Card Is Not Ready!";
	}

	public static String getMSISDN() { // phone number
		if (sbSimCardReady) {
			return sTelephonyManager.getLine1Number();
		}
		return "Your SIM Card Is Not Ready!";
	}

	public static void callTel(final String pTelNumber) {
		Uri telUri = Uri.parse("tel:" + pTelNumber);
		Intent intent = new Intent(Intent.ACTION_DIAL, telUri);
		thisApp.startActivity(intent);
	}

	public static String getTradeNo() {
		SimpleDateFormat format = new SimpleDateFormat("yyyyMMddHHmmss");
		Date date = new Date();
		String strKey = format.format(date);

		java.util.Random r = new java.util.Random();
		strKey = strKey + r.nextInt(10000);
		return strKey;
	}

	public static String getLocalMacAddress() {
		WifiInfo info = sWifiManager.getConnectionInfo();
		return info.getMacAddress();
	}

	public static String getLocalIpAddress() {
		try {
			for (Enumeration<NetworkInterface> en = NetworkInterface
					.getNetworkInterfaces(); en.hasMoreElements();) {
				NetworkInterface intf = en.nextElement();
				for (Enumeration<InetAddress> enumIpAddr = intf
						.getInetAddresses(); enumIpAddr.hasMoreElements();) {
					InetAddress inetAddress = enumIpAddr.nextElement();
					if (!inetAddress.isLoopbackAddress()) {
						return inetAddress.getHostAddress().toString();
					}
				}
			}
		} catch (SocketException ex) {
			Log.e("WifiPreference IpAddress", ex.toString());
		}
		return "";
	}

	private static boolean checkPermission(Context context,
			String permisiion_url) {
		PackageManager pm = context.getPackageManager();
		String packName = context.getApplicationContext().getPackageName();
		boolean permission = (PackageManager.PERMISSION_GRANTED == pm
				.checkPermission(permisiion_url, packName));
		return permission;
	}

	public static String getLocalIP() {
		try {
			// for (Enumeration<NetworkInterface> en = NetworkInterface
			// .getNetworkInterfaces(); en.hasMoreElements();) {
			// NetworkInterface intf = en.nextElement();
			// for (Enumeration<InetAddress> enumIpAddr = intf
			// .getInetAddresses(); enumIpAddr.hasMoreElements();) {
			// InetAddress inetAddress = enumIpAddr.nextElement();
			// if (!inetAddress.isLoopbackAddress()) {
			// return inetAddress.getHostAddress().toString();
			// }
			// }
			// }
			WifiManager wifiManager = (WifiManager) thisApp
					.getSystemService(Context.WIFI_SERVICE);
			WifiInfo wifiInfo = wifiManager.getConnectionInfo();
			int i = wifiInfo.getIpAddress();
			return int2ip(i);
		} catch (Exception ex) {
			return " 获取IP出错鸟!!!!请保证是WIFI,或者请重新打开网络!\n" + ex.getMessage();
		}
		// return null;
	}

	public static String int2ip(int ipInt) {
		StringBuilder sb = new StringBuilder();
		sb.append(ipInt & 0xFF).append(".");
		sb.append((ipInt >> 8) & 0xFF).append(".");
		sb.append((ipInt >> 16) & 0xFF).append(".");
		sb.append((ipInt >> 24) & 0xFF);
		return sb.toString();
	}

	// check network type
	private static boolean isFastMobileNetwork() {
//		TelephonyManager telephonyManager = (TelephonyManager) appCtx
//				.getSystemService(Context.TELEPHONY_SERVICE);
		switch (sTelephonyManager.getNetworkType()) {
		case TelephonyManager.NETWORK_TYPE_1xRTT:
			return false; // ~ 50-100 kbps
		case TelephonyManager.NETWORK_TYPE_CDMA:
			return false; // ~ 14-64 kbps
		case TelephonyManager.NETWORK_TYPE_EDGE:
			return false; // ~ 50-100 kbps
		case TelephonyManager.NETWORK_TYPE_EVDO_0:
			return true; // ~ 400-1000 kbps
		case TelephonyManager.NETWORK_TYPE_EVDO_A:
			return true; // ~ 600-1400 kbps
		case TelephonyManager.NETWORK_TYPE_GPRS:
			return false; // ~ 100 kbps
		case TelephonyManager.NETWORK_TYPE_HSDPA:
			return true; // ~ 2-14 Mbps
		case TelephonyManager.NETWORK_TYPE_HSPA:
			return true; // ~ 700-1700 kbps
		case TelephonyManager.NETWORK_TYPE_HSUPA:
			return true; // ~ 1-23 Mbps
		case TelephonyManager.NETWORK_TYPE_UMTS:
			return true; // ~ 400-7000 kbps
		case TelephonyManager.NETWORK_TYPE_EHRPD:
			return true; // ~ 1-2 Mbps
		case TelephonyManager.NETWORK_TYPE_EVDO_B:
			return true; // ~ 5 Mbps
		case TelephonyManager.NETWORK_TYPE_IDEN:
			return false; // ~25 kbps
		case TelephonyManager.NETWORK_TYPE_LTE:
			return true; // ~ 10+ Mbps
		case TelephonyManager.NETWORK_TYPE_UNKNOWN:
			return false;
		default:
			return false;
		}
	}

	/**
	 * 获取网络状态，wifi,wap,2g,3g.
	 * 
	 * @param context
	 *            上下文
	 * @return int 网络状态 {@link #NETWORKTYPE_2G},{@link #NETWORKTYPE_3G},          *
	 *         {@link #NETWORKTYPE_INVALID},{@link #NETWORKTYPE_WAP}*
	 *         <p>
	 *         {@link #NETWORKTYPE_WIFI}
	 */
	public static int getNetWorkType() {

		int networkType = NETWORKTYPE_INVALID;
		
		ConnectivityManager manager = (ConnectivityManager) appCtx
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo networkInfo = manager.getActiveNetworkInfo();

		if (networkInfo != null && networkInfo.isConnected()) {
			String type = networkInfo.getTypeName();

			if (type.equalsIgnoreCase("WIFI")) {
				networkType = NETWORKTYPE_WIFI;
			} else if (type.equalsIgnoreCase("MOBILE")) {
				String proxyHost = android.net.Proxy.getDefaultHost();

				networkType = TextUtils.isEmpty(proxyHost) ? (isFastMobileNetwork() ? NETWORKTYPE_3G
						: NETWORKTYPE_2G)
						: NETWORKTYPE_WAP;
			}
		} else {
			networkType = NETWORKTYPE_INVALID;
		}

		return networkType;
	}

}
